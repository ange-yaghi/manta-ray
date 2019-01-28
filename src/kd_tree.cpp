#include <kd_tree.h>

#include <mesh.h>
#include <standard_allocator.h>
#include <intersection_list.h>

#include <algorithm>

manta::KDTree::KDTree() {
	m_nodes = nullptr;
	m_nodeCapacity = 0;
	m_nodeCount = 0;

	m_volumeCount = 0;
	m_volumeCapacity = 0;

	m_faceLists = nullptr;
	m_faceCount = 0;
}

manta::KDTree::~KDTree() {
	assert(m_faceLists == nullptr);
	assert(m_nodeVolumes == nullptr);
	assert(m_nodes == nullptr);
}

void manta::KDTree::initialize(math::real width, const math::Vector &position) {
	m_width = width;

	m_bounds.maxPoint = math::loadScalar(width);
	m_bounds.minPoint = math::loadScalar(-width);
}

void manta::KDTree::destroy() {
	if (m_faceLists != nullptr) {
		StandardAllocator::Global()->free(m_faceLists, m_faceCount);
		m_faceLists = nullptr;
		m_faceCount = 0;
	}

	if (m_nodes != nullptr) {
		StandardAllocator::Global()->aligned_free(m_nodes, m_nodeCapacity);
		m_nodes = nullptr;
		m_nodeCapacity = 0;
		m_nodeCount = 0;
	}

	if (m_nodeVolumes != nullptr) {
		StandardAllocator::Global()->aligned_free(m_nodeVolumes, m_volumeCapacity);
		m_nodeVolumes = nullptr;
		m_volumeCount = 0;
		m_volumeCapacity = 0;
	}
}

bool manta::KDTree::findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const {
	math::real tmin, tmax;
	if (!m_bounds.rayIntersect(*ray, &tmin, &tmax)) {
		return false;
	}

	constexpr int MAX_DEPTH = 64;
	KDJob jobs[MAX_DEPTH];
	int currentJob = 0;

	bool hit = false;
	math::real closestHit = std::min(tmax, maxDepth);
	const KDTreeNode *node = &m_nodes[0];
	while (node != nullptr) {
		//int nodeIndex = node - m_nodes;

		if (closestHit < tmin) break;
		if (!node->isLeaf()) {
			int axis = node->getSplitAxis();
			math::real o_axis = math::get(ray->getSource(), axis);
			math::real tPlane = (node->getSplit() - o_axis) * math::get(ray->getInverseDirection(), axis);

			const KDTreeNode *firstChild, *secondChild;
			bool belowFirst = (o_axis < node->getSplit()) || (o_axis == node->getSplit() && math::get(ray->getDirection(), axis) <= 0);

			if (belowFirst) {
				firstChild = node + 1;
				secondChild = &m_nodes[node->getAboveChild()];
			}
			else {
				firstChild = &m_nodes[node->getAboveChild()];
				secondChild = node + 1;
			}

			if (tPlane > tmax || tPlane <= 0) {
				node = firstChild;
			}
			else if (tPlane < tmin) {
				node = secondChild;
			}
			else {
				// Add second child to queue
				jobs[currentJob].node = secondChild;
				jobs[currentJob].tmin = tPlane;
				jobs[currentJob].tmax = tmax;
				++currentJob;

				node = firstChild;
				tmax = tPlane;
			}
		}
		else {
			int primitiveCount = node->getPrimitiveCount();
			int objectOffset = node->getObjectOffset();
			int *faceList;
			if (primitiveCount > 0) {
				const KDBoundingVolume &bv = m_nodeVolumes[objectOffset];
				faceList = &m_faceLists[bv.faceListOffset];

				math::real t0, t1;
				if (bv.bounds.rayIntersect(*ray, &t0, &t1)) {
					if (m_mesh->findClosestIntersection(faceList, primitiveCount, ray, intersection, minDepth, closestHit, s)) {
						hit = true;
						closestHit = intersection->depth;
					}
				}
			}
			else {

			}

			if (currentJob > 0) {
				--currentJob;
				node = jobs[currentJob].node;
				tmin = jobs[currentJob].tmin;
				tmax = jobs[currentJob].tmax;
			}
			else break;
		}
	}

	return hit;
}

manta::math::Vector manta::KDTree::getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const {
	return math::Vector();
}

void manta::KDTree::getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {

}

void manta::KDTree::fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const {
	hint->sceneGeometry->fineIntersection(r, p, hint);
}

bool manta::KDTree::fastIntersection(const LightRay *ray) const {
	return true;
}

void manta::KDTree::analyze(Mesh *mesh, int maxSize) {
	constexpr math::real maxDepth = 64;

	m_mesh = mesh;

	std::vector<int> faces;
	int nFaces = mesh->getFaceCount();
	for (int i = 0; i < nFaces; i++) {
		faces.push_back(i);
	}

	KDTreeWorkspace workspace;
	workspace.mesh = mesh;
	workspace.maxPrimitives = maxSize;
	
	for (int i = 0; i < 3; i++) {
		//workspace.edges[i] = new KDBoundEdge[mesh->getFaceCount() * 2];
		workspace.edges[i] = StandardAllocator::Global()->allocate<KDBoundEdge>(mesh->getFaceCount() * 2);
	}

	// Generate bounds for all faces
	//workspace.allFaceBounds = new AABB[nFaces];
	workspace.allFaceBounds = StandardAllocator::Global()->allocate<AABB>(nFaces, 16);
	for (int i = 0; i < nFaces; i++) {
		Face *face = &mesh->getFaces()[i];

		Triangle triangle;
		triangle.u = *mesh->getVertex(face->u);
		triangle.v = *mesh->getVertex(face->v);
		triangle.w = *mesh->getVertex(face->w);

		calculateAABB(&triangle, &workspace.allFaceBounds[i]);
	}

	AABB topNodeBounds;
	topNodeBounds.minPoint = math::loadScalar(-m_width);
	topNodeBounds.maxPoint = math::loadScalar(m_width);

	int badRefines = 0;
	analyze(0, &topNodeBounds, faces, badRefines, maxDepth, &workspace);

	// Copy faces into a new array
	int totalFaces = workspace.faces.size();
	//m_faceLists = new int[totalFaces];
	m_faceLists = StandardAllocator::Global()->allocate<int>(totalFaces);
	m_faceCount = totalFaces;
	for (int i = 0; i < totalFaces; i++) {
		m_faceLists[i] = workspace.faces[i];
	}

	// Destroy all allocated memory
	for (int i = 0; i < 3; i++) {
		StandardAllocator::Global()->free(workspace.edges[i], mesh->getFaceCount() * 2);
	}

	StandardAllocator::Global()->aligned_free(workspace.allFaceBounds, nFaces);
}

void manta::KDTree::analyze(int currentNode, AABB *nodeBounds, const std::vector<int> &faces, int badRefines, int depth, KDTreeWorkspace *workspace) {
	constexpr math::real intersectionCost = 80;
	constexpr math::real traversalCost = 1;
	constexpr math::real emptyBonus = 0.5;

	createNode();

	// Debug only
	m_nodeBounds.push_back(*nodeBounds);

	int primitiveCount = (int)faces.size();
	if (primitiveCount <= workspace->maxPrimitives || depth == 0) {
		initLeaf(currentNode, faces, workspace);
		return;
	}

	int bestAxis = -1, bestOffset = -1;
	math::real bestCost = math::constants::REAL_MAX;
	math::real oldCost = intersectionCost * primitiveCount;
	math::real totalSA = nodeBounds->surfaceArea();
	math::real invTotalSA = 1 / totalSA;
	math::Vector d = math::sub(nodeBounds->maxPoint, nodeBounds->minPoint);

	int retries = 0;
	int axis = math::maxDimension3(d);

	retry_split:
	// Initialize edges
	for (int i = 0; i < primitiveCount; i++) {
		int primitiveIndex = faces[i];
		const AABB &bounds = workspace->allFaceBounds[primitiveIndex];
		workspace->edges[axis][2 * i] = KDBoundEdge(math::get(bounds.minPoint, axis), primitiveIndex, true);	// Start
		workspace->edges[axis][2 * i + 1] = KDBoundEdge(math::get(bounds.maxPoint, axis), primitiveIndex, false);	// End
	}

	std::sort(&workspace->edges[axis][0], &workspace->edges[axis][2 * primitiveCount],
		[](const KDBoundEdge &e0, const KDBoundEdge &e1) -> bool {
		if (e0.t == e1.t) return (int)e0.edgeType < (int)e1.edgeType;
		else return e0.t < e1.t;
	});

	int nBelow = 0, nAbove = primitiveCount;
	for (int i = 0; i < 2 * primitiveCount; i++) {
		if (workspace->edges[axis][i].edgeType == KDBoundEdge::EdgeType::End) --nAbove;
		math::real edgeT = workspace->edges[axis][i].t;
		if (edgeT > math::get(nodeBounds->minPoint, axis) && edgeT < math::get(nodeBounds->maxPoint, axis)) {
			// Compute split cost
			int otherAxis0 = (axis + 1) % 3, otherAxis1 = (axis + 2) % 3;
			math::real belowSA = 2 * (math::get(d, otherAxis0) * math::get(d, otherAxis1) +
				(edgeT - math::get(nodeBounds->minPoint, axis)) *
				(math::get(d, otherAxis0) + math::get(d, otherAxis1)));
			math::real aboveSA = 2 * (math::get(d, otherAxis0) * math::get(d, otherAxis1) +
				(math::get(nodeBounds->maxPoint, axis) - edgeT) *
				(math::get(d, otherAxis0) + math::get(d, otherAxis1)));

			math::real pBelow = belowSA * invTotalSA;
			math::real pAbove = aboveSA * invTotalSA;
			math::real eb = (nAbove == 0 || nBelow == 0) ? emptyBonus : 0;
			math::real cost = traversalCost + intersectionCost * (1 - eb) * (pBelow * nBelow + pAbove * nAbove);

			if (cost < bestCost) {
				bestCost = cost;
				bestAxis = axis;
				bestOffset = i;
			}
		}
		if (workspace->edges[axis][i].edgeType == KDBoundEdge::EdgeType::Start) ++nBelow;
	}

	if (bestAxis == -1 && retries < 2) {
		++retries;
		axis = (axis + 1) % 3;
		goto retry_split;
	}

	if (bestCost > oldCost) ++badRefines;
	if (((bestCost > 4 * oldCost && primitiveCount < workspace->maxPrimitives) || bestAxis == -1 || badRefines == 3)) {
		initLeaf(currentNode, faces, workspace);
		return;
	}

	// Classify primitives
	std::vector<int> primitives0;
	std::vector<int> primitives1;
	for (int i = 0; i < bestOffset; ++i) {
		if (workspace->edges[axis][i].edgeType == KDBoundEdge::EdgeType::Start) {
			primitives0.push_back(workspace->edges[axis][i].primitiveIndex);
		}
	}
	for (int i = bestOffset + 1; i < 2 * primitiveCount; ++i) {
		if (workspace->edges[axis][i].edgeType == KDBoundEdge::EdgeType::End) {
			primitives1.push_back(workspace->edges[axis][i].primitiveIndex);
		}
	}

	// Split
	math::real split = workspace->edges[axis][bestOffset].t;
	AABB bounds0 = *nodeBounds, bounds1 = *nodeBounds;
	math::set(bounds0.maxPoint, bestAxis, split);
	math::set(bounds1.minPoint, bestAxis, split);

	analyze(currentNode + 1, &bounds0, primitives0, badRefines, depth - 1, workspace);
	int aboveChild = m_nodeCount;
	m_nodes[currentNode].initInterior(bestAxis, aboveChild, split);
	analyze(aboveChild, &bounds1, primitives1, badRefines, depth - 1, workspace);
}

int manta::KDTree::createNode() {
	if (m_nodeCount == m_nodeCapacity) {
		int newSize = 1;
		if (m_nodeCapacity > 0) newSize = m_nodeCapacity * 2;

		KDTreeNode *newNodes = StandardAllocator::Global()->allocate<KDTreeNode>(newSize, 16);
		if (m_nodeCount > 0) {
			// Copy old nodes
			memcpy((void *)newNodes, (void *)m_nodes, sizeof(KDTreeNode) * m_nodeCount);
		}

		if (m_nodeCapacity > 0) {
			StandardAllocator::Global()->aligned_free(m_nodes, m_nodeCapacity);
		}

		m_nodes = newNodes;
		m_nodeCapacity = newSize;
	}

	int newNode = ++m_nodeCount;
	return newNode;
}

int manta::KDTree::createNodeVolume() {
	if (m_volumeCount == m_volumeCapacity) {
		int newSize = 1;
		if (m_volumeCapacity > 0) newSize = m_volumeCapacity * 2;

		KDBoundingVolume *newVolumes = StandardAllocator::Global()->allocate<KDBoundingVolume>(newSize, 16);
		if (m_nodeCount > 0) {
			// Copy old nodes
			memcpy((void *)newVolumes, (void *)m_nodeVolumes, sizeof(KDBoundingVolume) * m_volumeCount);
		}

		if (m_volumeCapacity > 0) {
			StandardAllocator::Global()->aligned_free(m_nodeVolumes, m_volumeCapacity);
		}

		m_nodeVolumes = newVolumes;
		m_volumeCapacity = newSize;
	}

	int newVolume = m_volumeCount++;
	return newVolume;
}

void manta::KDTree::initLeaf(int node, const std::vector<int> &faces, KDTreeWorkspace *workspace) {
	int primitiveCount = faces.size();
	int newNodeVolume = createNodeVolume();
	m_nodes[node].initLeaf(primitiveCount, newNodeVolume);

	if (primitiveCount > 0) {
		// Initialize the new volume
		KDBoundingVolume &volume = m_nodeVolumes[newNodeVolume];
		volume.faceListOffset = workspace->faces.size();
		volume.bounds = workspace->allFaceBounds[faces[0]];

		// Add all primitives to the list
		for (int i = 0; i < primitiveCount; i++) {
			workspace->faces.push_back(faces[i]);
		}

		// Merge all bounds
		for (int i = 1; i < primitiveCount; i++) {
			volume.bounds.merge(workspace->allFaceBounds[faces[i]]);
		}
	}
}

void manta::KDTree::writeToObjFile(const char *fname) const {
	std::ofstream f(fname);
	
	math::real depth;
	int nodeCount = m_nodeCount;
	for (int i = 0; i < nodeCount; i++) {
		const AABB &bounds = m_nodeBounds[i];
		KDTreeNode &node = m_nodes[i];

		int faceCount = 0;
		if (node.getSplitAxis() == 0x3) faceCount = node.getPrimitiveCount();

		if (faceCount != 0) f << "o " << "LEAF_" << i << "_N" << faceCount << std::endl;
		else f << "o " << "LEAF_" << i << "_N" << 0 << std::endl;

		math::Vector minPoint = bounds.minPoint;
		math::Vector maxPoint = bounds.maxPoint;

		int vertexOffset = i * 8 + 1;
		f << "v " << math::getX(minPoint) << " " << math::getY(minPoint) << " " << math::getZ(minPoint) << std::endl;
		f << "v " << math::getX(maxPoint) << " " << math::getY(minPoint) << " " << math::getZ(minPoint) << std::endl;
		f << "v " << math::getX(minPoint) << " " << math::getY(minPoint) << " " << math::getZ(maxPoint) << std::endl;
		f << "v " << math::getX(maxPoint) << " " << math::getY(minPoint) << " " << math::getZ(maxPoint) << std::endl;
		f << "v " << math::getX(minPoint) << " " << math::getY(maxPoint) << " " << math::getZ(minPoint) << std::endl;
		f << "v " << math::getX(maxPoint) << " " << math::getY(maxPoint) << " " << math::getZ(minPoint) << std::endl;
		f << "v " << math::getX(minPoint) << " " << math::getY(maxPoint) << " " << math::getZ(maxPoint) << std::endl;
		f << "v " << math::getX(maxPoint) << " " << math::getY(maxPoint) << " " << math::getZ(maxPoint) << std::endl;

		f << "f " << vertexOffset << " " << vertexOffset + 1 << " " << vertexOffset + 3 << " " << vertexOffset + 2 << std::endl;
		f << "f " << vertexOffset << " " << vertexOffset + 4 << " " << vertexOffset + 6 << " " << vertexOffset + 2 << std::endl;
		f << "f " << vertexOffset << " " << vertexOffset + 4 << " " << vertexOffset + 5 << " " << vertexOffset + 1 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 5 << " " << vertexOffset + 1 << " " << vertexOffset + 3 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 6 << " " << vertexOffset + 2 << " " << vertexOffset + 3 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 6 << " " << vertexOffset + 4 << " " << vertexOffset + 5 << std::endl;
	}
}
