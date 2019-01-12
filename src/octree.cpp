#include <octree.h>

#include <scene_object.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_list.h>
#include <stack_list.h>
#include <standard_allocator.h>

#include <assert.h>
#include <math.h>
#include <fstream>

manta::Octree::Octree() {
	m_mesh = nullptr;
}

manta::Octree::~Octree() {
}

void manta::Octree::initialize(math::real width, const math::Vector &position) {
	m_position = position;

	math::real epsWidth = (math::real)(1E-4) + width;

	m_tree.maxPoint = math::add(m_position, math::loadVector(epsWidth, epsWidth, epsWidth));
	m_tree.minPoint = math::sub(m_position, math::loadVector(epsWidth, epsWidth, epsWidth));
}

void manta::Octree::destroy() {
	/*
	if (m_mesh != nullptr) {
		m_mesh->destroy();
		StandardAllocator::Global()->aligned_free(m_mesh);
		m_mesh = nullptr;
	}
	if (m_children != nullptr) {
		for (int i = 0; i < m_childCount; i++) {
			m_children[i].destroy();
		}
		StandardAllocator::Global()->aligned_free(m_children, 8);
		m_children = nullptr;
	}*/
}

bool manta::Octree::findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const {
	math::Vector ood = math::div(math::constants::One, ray->getDirection());
	return findClosestIntersection(&m_tree, ray, ood, intersection, minDepth, maxDepth, s, true);
}

manta::math::Vector manta::Octree::getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const {
	return hint->sceneGeometry->getClosestPoint(hint, p);
}

void manta::Octree::getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {
	getVicinity(&m_tree, p, radius, list, object);
}

void manta::Octree::fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const {
	hint->sceneGeometry->fineIntersection(r, p, hint);
}

bool manta::Octree::fastIntersection(const LightRay * ray) const {
	return true;
}

void manta::Octree::analyze(Mesh *mesh, int maxSize) {
	std::vector<int> allFaces;
	int faceCount = mesh->getFaceCount();

	m_mesh = mesh;

	for (int i = 0; i < faceCount; i++) {
		allFaces.push_back(i);
	}

	analyze(mesh, &m_tree, maxSize, allFaces);
	shrink(&m_tree);

	// Copy data from vectors
	int childListsCount = m_childListsTemp.size();
	int faceListsCount = m_faceListsTemp.size();

	m_childLists = StandardAllocator::Global()->allocate<OctreeBV *>(childListsCount);
	m_faceLists = StandardAllocator::Global()->allocate<int *>(faceListsCount);

	for (int i = 0; i < childListsCount; i++) {
		m_childLists[i] = m_childListsTemp[i];
	}

	for (int i = 0; i < faceListsCount; i++) {
		m_faceLists[i] = m_faceListsTemp[i];
	}
}

void manta::Octree::writeToObjFile(const char *fname, const LightRay *ray) const {
	std::ofstream f(fname);

	assert(f.is_open());

	int currentLeaf = 0;
	math::Vector ood = math::constants::Zero;
	if (ray != nullptr) ood = math::div(math::constants::One, ray->getDirection());
	writeToObjFile(&m_tree, f, currentLeaf, ray, ood);

	f.close();
}

int manta::Octree::countFaces() const {
	return countFaces(&m_tree);
}

int manta::Octree::countLeaves() const {
	return countLeaves(&m_tree);
}

int manta::Octree::countFaces(const OctreeBV *leaf) const {
	int faceCount = 0;
	int childCount = leaf->childCount;

	if (childCount > 0) {
		OctreeBV *children = m_childLists[leaf->childList];
		for (int i = 0; i < childCount; i++) {
			faceCount += countFaces(&children[i]);
		}
	}

	faceCount += leaf->faceCount;

	return faceCount;
}

int manta::Octree::countLeaves(const OctreeBV *leaf) const {
	int leafCount = 0;
	int childCount = leaf->childCount;

	if (childCount > 0) {
		OctreeBV *children = m_childLists[leaf->childList];
		for (int i = 0; i < childCount; i++) {
			leafCount += countLeaves(&children[i]);
		}
	}

	if (m_mesh != nullptr) {
		leafCount += 1;
	}

	return leafCount;
}

void manta::Octree::writeToObjFile(const OctreeBV *leaf, std::ofstream &f, int &currentLeaf, const LightRay *ray, const math::Vector &ood) const {
	math::real depth;
	if ((leaf->childCount != 0 || leaf->faceCount != 0) && (ray == nullptr || AABBIntersect(leaf, ray, &depth, ood))) {
		if (leaf->faceCount != 0) f << "o " << "LEAF_" << currentLeaf << "_N" << leaf->faceCount << std::endl;
		else f << "o " << "LEAF_" << currentLeaf << "_N" << 0 << std::endl;

		math::Vector minPoint = leaf->minPoint;
		math::Vector maxPoint = leaf->maxPoint;

		int vertexOffset = currentLeaf * 8 + 1;
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

		currentLeaf++;

		int childCount = leaf->childCount;
		for (int i = 0; i < childCount; i++) {
			writeToObjFile(&m_childLists[leaf->childList][i], f, currentLeaf, ray, ood);
		}
	}
}

bool manta::Octree::findClosestIntersection(const OctreeBV *leaf, const LightRay *ray, const math::Vector &ood, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s, bool skip) const {
	math::real currentMaxDepth = maxDepth;
	math::real rayDepth;
	
	bool found = false;
	if (skip || AABBIntersect(leaf, ray, &rayDepth, ood)) {
		if (rayDepth > currentMaxDepth) return false;

		if (leaf->faceCount > 0) {
			bool foundInMesh = m_mesh->findClosestIntersection(m_faceLists[leaf->faceList], leaf->faceCount, ray, intersection, minDepth, currentMaxDepth, s);
			if (foundInMesh) {
				found = true;
				currentMaxDepth = intersection->depth;
			}
		}

		int childCount = leaf->childCount;
		OctreeBV *childList = m_childLists[leaf->childList];
		for (int i = 0; i < childCount; i++) {
			bool foundInChild = findClosestIntersection(&childList[i], ray, ood, intersection, minDepth, currentMaxDepth, s);
			if (foundInChild) {
				found = true;
				currentMaxDepth = intersection->depth;
			}
		}
	}
	return found;
}

void manta::Octree::getVicinity(const OctreeBV *leaf, const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {
	if (AABBIntersect(leaf, p, radius)) {
		m_mesh->getVicinity(m_faceLists[leaf->faceList], leaf->faceCount, p, radius, list, object);

		int childList = leaf->childList;
		for (int i = 0; i < leaf->childCount; i++) {
			getVicinity(&m_childLists[childList][i], p, radius, list, object);
		}
	}
}

bool manta::Octree::analyze(Mesh *mesh, OctreeBV *leaf, int maxSize, std::vector<int> &facePool) {
	/*
	Face *faces = mesh->getFaces();

	int faceCount = (parent == nullptr) ? (int)mesh->getFaceCount() : (int)parent->m_tempFaces.size();

	for (int i = 0; i < faceCount; i++) {
		int faceIndex = (parent == nullptr) ? i : parent->m_tempFaces[i].face;

		math::Vector v1 = mesh->getVertices()[faces[faceIndex].u];
		math::Vector v2 = mesh->getVertices()[faces[faceIndex].v];
		math::Vector v3 = mesh->getVertices()[faces[faceIndex].w];

		//if (!checkVertex(v1, 1E-5)) continue;
		//if (!checkVertex(v2, 1E-5)) continue;
		//if (!checkVertex(v3, 1E-5)) continue;

		if (!checkTriangle(v1, v2, v3))
			continue;
		else if (m_width < 1e-6) {
			checkTriangle(v1, v2, v3);
		}

		TempFace f;
		f.face = faceIndex;
		f.presentInChild = false;
		m_tempFaces.push_back(f);
		if (parent != nullptr) {
			parent->m_tempFaces[i].presentInChild = true;
		}
	}

	if (getUsageInternal() > maxSize && m_width > 1E-4) {
		m_children = StandardAllocator::Global()->allocate<Octree>(8, 16);
		m_childCount = 8;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					math::real halfWidth = m_width / (math::real)2.0;
					math::Vector offset = math::loadVector((2 * i - 1) * halfWidth, (2 * j - 1) * halfWidth, (2 * k - 1) * halfWidth);
					m_children[i * (2 * 2) + j * 2 + k].initialize(halfWidth, math::add(m_position, offset));
				}
			}
		}

		for (int i = 0; i < 8; i++) {
			m_children[i].analyze(mesh, this, maxSize);
		}
	}

	int *vertexMap = nullptr;
	int *normalMap = nullptr;
	int *textureMap = nullptr;
	int internalFaceCount = (int)m_tempFaces.size();
	int realVertexCount = 0;
	int realNormalCount = 0;
	int realTextureCount = 0;
	int realFaceCount = 0;

	if (getUsageInternal() > 0) {
		vertexMap = StandardAllocator::Global()->allocate<int>(mesh->getVertexCount());
		if (mesh->getNormalCount() > 0) normalMap = StandardAllocator::Global()->allocate<int>(mesh->getNormalCount());
		if (mesh->getTexCoordCount() > 0) textureMap = StandardAllocator::Global()->allocate<int>(mesh->getTexCoordCount());
		for (int i = 0; i < mesh->getVertexCount(); i++) vertexMap[i] = -1;
		for (int i = 0; i < mesh->getNormalCount(); i++) normalMap[i] = -1;
		for (int i = 0; i < mesh->getTexCoordCount(); i++) textureMap[i] = -1;

		for (int i = 0; i < internalFaceCount; i++) {
			if (!m_tempFaces[i].presentInChild) {
				Face *face = &mesh->getFaces()[m_tempFaces[i].face];
				int u = face->u;
				int v = face->v;
				int w = face->w;

				if (normalMap != nullptr) {
					int nu = face->nu;
					int nv = face->nv;
					int nw = face->nw;

					if (normalMap[nu] == -1) {
						normalMap[nu] = realNormalCount;
						realNormalCount++;
					}
					if (normalMap[nv] == -1) {
						normalMap[nv] = realNormalCount;
						realNormalCount++;
					}
					if (normalMap[nw] == -1) {
						normalMap[nw] = realNormalCount;
						realNormalCount++;
					}
				}

				if (textureMap != nullptr) {
					int tu = face->tu;
					int tv = face->tv;
					int tw = face->tw;

					if (tu != -1 && textureMap[tu] == -1) {
						textureMap[tu] = realTextureCount;
						realTextureCount++;
					}
					if (tv != -1 && textureMap[tv] == -1) {
						textureMap[tv] = realTextureCount;
						realTextureCount++;
					}
					if (tw != -1 && textureMap[tw] == -1) {
						textureMap[tw] = realTextureCount;
						realTextureCount++;
					}
				}

				if (vertexMap[u] == -1) {
					vertexMap[u] = realVertexCount;
					realVertexCount++;
				}
				if (vertexMap[v] == -1) {
					vertexMap[v] = realVertexCount;
					realVertexCount++;
				}
				if (vertexMap[w] == -1) {
					vertexMap[w] = realVertexCount;
					realVertexCount++;
				}
				realFaceCount++;
			}
		}
	}

	if (realFaceCount > 0) {
		m_mesh = StandardAllocator::Global()->allocate<Mesh>(1, 16);
		m_mesh->initialize(realFaceCount, realVertexCount, realNormalCount, realTextureCount);
		m_mesh->setPerVertexNormals(realNormalCount > 0);
		m_mesh->setUseTexCoords(realTextureCount > 0);

		math::Vector *newVertices = m_mesh->getVertices();
		math::Vector *newNormals = m_mesh->getNormals();
		math::Vector *newTexCoords = m_mesh->getTexCoords();
		Face *newFaces = m_mesh->getFaces();

		int currentFace = 0;
		for (int i = 0; i < internalFaceCount; i++) {
			if (!m_tempFaces[i].presentInChild) {
				Face *face = &mesh->getFaces()[m_tempFaces[i].face];
				int u = face->u;
				int v = face->v;
				int w = face->w;

				if (normalMap != nullptr) {
					int nu = face->nu;
					int nv = face->nv;
					int nw = face->nw;

					newNormals[normalMap[nu]] = mesh->getNormals()[nu];
					newNormals[normalMap[nv]] = mesh->getNormals()[nv];
					newNormals[normalMap[nw]] = mesh->getNormals()[nw];

					newFaces[currentFace].nu = normalMap[nu];
					newFaces[currentFace].nv = normalMap[nv];
					newFaces[currentFace].nw = normalMap[nw];
				}

				if (textureMap != nullptr) {
					int tu = face->tu;
					int tv = face->tv;
					int tw = face->tw;

					if (tu != -1) newTexCoords[textureMap[tu]] = mesh->getTexCoords()[tu];
					if (tv != -1) newTexCoords[textureMap[tv]] = mesh->getTexCoords()[tv];
					if (tw != -1) newTexCoords[textureMap[tw]] = mesh->getTexCoords()[tw];

					if (tu != -1) newFaces[currentFace].tu = textureMap[tu];
					else newFaces[currentFace].tu = -1;

					if (tv != -1) newFaces[currentFace].tv = textureMap[tv];
					else newFaces[currentFace].tv = -1;

					if (tw != -1) newFaces[currentFace].tw = textureMap[tw];
					else newFaces[currentFace].tw = -1;
				}

				newVertices[vertexMap[u]] = mesh->getVertices()[u];
				newVertices[vertexMap[v]] = mesh->getVertices()[v];
				newVertices[vertexMap[w]] = mesh->getVertices()[w];

				newFaces[currentFace].u = vertexMap[u];
				newFaces[currentFace].v = vertexMap[v];
				newFaces[currentFace].w = vertexMap[w];
				
				newFaces[currentFace].material = face->material;
				newFaces[currentFace].globalId = face->globalId;

				currentFace++;
			}
		}
		m_mesh->precomputeValues();
	}

	m_tempFaces.clear();

	if (vertexMap != nullptr) StandardAllocator::Global()->free(vertexMap, mesh->getVertexCount());
	if (normalMap != nullptr) StandardAllocator::Global()->free(normalMap, mesh->getNormalCount());
	if (textureMap != nullptr) StandardAllocator::Global()->free(textureMap, mesh->getTexCoordCount());
	*/
	Face *faces = mesh->getFaces();
	math::Vector *vertices = mesh->getVertices();
	int faceCount = facePool.size();

	std::vector<int> enclosedFaces;

	for (int i = 0; i < faceCount; i++) {
		Face &face = faces[facePool[i]];
		math::Vector v1, v2, v3;

		v1 = vertices[face.u];
		v2 = vertices[face.v];
		v3 = vertices[face.w];

		if (checkTriangle(leaf, v1, v2, v3)) {
			enclosedFaces.push_back(facePool[i]);
		}
	}

	int enclosedFaceCount = enclosedFaces.size();

	math::Vector halfSize = math::mul(math::sub(leaf->maxPoint, leaf->minPoint), math::constants::Half);
	math::Vector halfHalfSize = math::mul(halfSize, math::constants::Half);
	math::Vector position = math::mul(math::add(leaf->maxPoint, leaf->minPoint), math::constants::Half);

	if ((enclosedFaceCount <= maxSize || math::getScalar(halfSize) < 1E-4) && enclosedFaceCount > 0) {
		int *newFaceArray = StandardAllocator::Global()->allocate<int>(enclosedFaceCount);
		for (int i = 0; i < enclosedFaceCount; i++) {
			newFaceArray[i] = enclosedFaces[i];
		}
		int index = m_faceListsTemp.size();
		m_faceListsTemp.push_back(newFaceArray);

		leaf->faceList = index;
		leaf->faceCount = enclosedFaceCount;
		leaf->childCount = 0;
		leaf->childList = 0;

		return true;
	}
	else if (enclosedFaceCount > 0) {
		// Create new children

		std::vector<OctreeBV> tempChildren;

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					OctreeBV newChild;
					math::Vector offset = math::mul(
						math::loadVector((2 * i - 1), (2 * j - 1), (2 * k - 1)),
						halfHalfSize);
					math::Vector childPosition = math::add(position, offset);

					newChild.maxPoint = math::add(childPosition, halfHalfSize);
					newChild.minPoint = math::sub(childPosition, halfHalfSize);

					bool containsFaces = analyze(mesh, &newChild, maxSize, enclosedFaces);
					if (containsFaces) {
						tempChildren.push_back(newChild);
					}
				}
			}
		}

		constexpr bool MERGE_SINGLE_CHILDREN = true;
		int childCount = tempChildren.size();
		if (childCount == 0) {
			// Shouldn't happen

		}
		else if (childCount > 1 || !MERGE_SINGLE_CHILDREN) {
			OctreeBV *childList = StandardAllocator::Global()->allocate<OctreeBV>(childCount, 16);
			int childListIndex = m_childListsTemp.size();
			m_childListsTemp.push_back(childList);

			for (int i = 0; i < childCount; i++) {
				childList[i] = tempChildren[i];
			}

			leaf->faceList = 0;
			leaf->faceCount = 0;
			leaf->childCount = childCount;
			leaf->childList = childListIndex;
		}
		else if (childCount == 1) {
			*leaf = tempChildren[0];
		}

		return true;
	}
	else {
		return false;
	}
}

void manta::Octree::shrink(OctreeBV *leaf) {
	int childCount = leaf->childCount;
	int childListIndex = leaf->childList;
	OctreeBV *childList = m_childListsTemp[childListIndex];
	for (int i = 0; i < childCount; i++) {
		shrink(&childList[i]);
	}
	
	math::Vector childMax;
	math::Vector childMin;
	for (int i = 0; i < childCount; i++) {
		if (i == 0) {
			childMax = childList[i].maxPoint;
			childMin = childList[i].minPoint;
		}
		else {
			childMax = math::componentMax(childMax, childList[i].maxPoint);
			childMin = math::componentMin(childMin, childList[i].minPoint);
		}
	}

	math::Vector vertexMax;
	math::Vector vertexMin;
	if (leaf->faceCount > 0) {
		int *faces = m_faceListsTemp[leaf->faceList];

		for (int i = 0; i < leaf->faceCount; i++) {
			const Face *face = m_mesh->getFace(faces[i]);
			const math::Vector &vu = *m_mesh->getVertex(face->u);
			const math::Vector &vv = *m_mesh->getVertex(face->v);
			const math::Vector &vw = *m_mesh->getVertex(face->w);
			if (i == 0) {
				vertexMax = math::componentMax(vu, vv);
				vertexMax = math::componentMax(vertexMax, vw);
				vertexMin = math::componentMin(vu, vv);
				vertexMin = math::componentMin(vertexMin, vw);
			}
			else {
				vertexMax = math::componentMax(vertexMax, vu);
				vertexMax = math::componentMax(vertexMax, vv);
				vertexMax = math::componentMax(vertexMax, vw);
				vertexMin = math::componentMin(vertexMin, vu);
				vertexMin = math::componentMin(vertexMin, vv);
				vertexMin = math::componentMin(vertexMin, vw);
			}
		}

		math::Vector localMax = math::componentMin(vertexMax, leaf->maxPoint);
		math::Vector localMin = math::componentMax(vertexMin, leaf->minPoint);
		if (childCount > 0) {
			leaf->maxPoint = math::componentMax(localMax, childMax);
			leaf->minPoint = math::componentMin(localMin, childMin);
		}
		else {
			leaf->maxPoint = localMax;
			leaf->minPoint = localMin;
		}
	}
	else {
		if (leaf->childCount > 0) {
			leaf->maxPoint = childMax;
			leaf->minPoint = childMin;
		}
		else {
			// Shouldn't happen
		}
	}

	const math::Vector eps = math::loadScalar((math::real)(1E-4));

	leaf->maxPoint = math::add(leaf->maxPoint, eps);
	leaf->minPoint = math::sub(leaf->minPoint, eps);
}

void manta::Octree::deleteChild(int childIndex) {
	/*
	m_children[childIndex].destroy();

	if (childIndex != m_childCount - 1) {
		m_children[childIndex] = m_children[m_childCount - 1];
		m_children[m_childCount - 1].clear();
	}

	m_childCount--;
	*/
}

void manta::Octree::clear() {
	/*
	m_childCount = 0;
	m_children = nullptr;
	m_mesh = nullptr;
	*/
}

bool manta::Octree::checkVertex(const OctreeBV *leaf, const math::Vector &v, math::real epsilon) const {
	math::Vector dmax = math::sub(v, leaf->maxPoint);
	if (math::getX(dmax) > epsilon) {
		return false;
	}
	if (math::getY(dmax) > epsilon) {
		return false;
	}
	if (math::getZ(dmax) > epsilon) {
		return false;
	}

	math::Vector dmin = math::sub(leaf->minPoint, v);
	if (math::getX(dmin) > epsilon) {
		return false;
	}
	if (math::getY(dmin) > epsilon) {
		return false;
	}
	if (math::getZ(dmin) > epsilon) {
		return false;
	}

	return true;
}

bool manta::Octree::checkPlane(const OctreeBV *leaf, const math::Vector &n, math::real d) const {
	math::Vector c = math::mul(math::add(leaf->maxPoint, leaf->minPoint), math::constants::Half);
	math::Vector e = math::sub(leaf->maxPoint, c);

	math::real r = math::getX(e) * abs(math::getX(n)) + math::getY(e) * abs(math::getY(n)) + math::getZ(e) * abs(math::getZ(n));
	math::real s = math::getScalar(math::dot(n, c)) - d;

	return abs(s) <= r;
}

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

bool manta::Octree::checkTriangle(const OctreeBV *leaf, const math::Vector &v0, const math::Vector &v1, const math::Vector &v2) const {
	math::real p0, p1, p2, r;

	math::Vector c = math::mul(math::add(leaf->maxPoint, leaf->minPoint), math::constants::Half);
	math::Vector extents = math::mul(math::sub(leaf->maxPoint, leaf->minPoint), math::constants::Half);
	math::real e0 = math::getX(extents);
	math::real e1 = math::getY(extents);
	math::real e2 = math::getZ(extents);

	math::Vector v0_rel = math::sub(v0, c);
	math::Vector v1_rel = math::sub(v1, c);
	math::Vector v2_rel = math::sub(v2, c);

	math::Vector f0 = math::sub(v1_rel, v0_rel);
	math::Vector f1 = math::sub(v2_rel, v1_rel);
	math::Vector f2 = math::sub(v0_rel, v2_rel);

	math::Vector u0 = math::constants::XAxis;
	math::Vector u1 = math::constants::YAxis;
	math::Vector u2 = math::constants::ZAxis;

	// Naive implementation for now
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			math::Vector u, f, altF;
			if (i == 0) u = u0; 
			else if (i == 1) u = u1;
			else if (i == 2) u = u2;

			if (j == 0) {
				f = f0;
				altF = f1;
			}
			else if (j == 1) {
				f = f1;
				altF = f2;
			}
			else if (j == 2) {
				f = f2;
				altF = f0;
			}

			math::Vector a = math::cross(u, f);

			if (abs(math::getX(a)) < 1e-6 && abs(math::getY(a) < 1e-6) && abs(math::getZ(a) < 1e-6)) {
				math::Vector n = math::cross(f, altF);
				a = math::cross(n, f);
			}

			r = e0 * abs(math::getScalar(math::dot(u0, a))) + e1 * abs(math::getScalar(math::dot(u1, a))) + e2 * abs(math::getScalar(math::dot(u2, a)));

			p0 = math::getScalar(math::dot(v0_rel, a));
			p1 = math::getScalar(math::dot(v1_rel, a));
			p2 = math::getScalar(math::dot(v2_rel, a));

			math::real maxp = max(max(p0, p1), p2);
			math::real minp = min(min(p0, p1), p2);

			if (minp > r) return false;
			if (maxp < -r) return false;
		}
	}

	math::real maxX = max(max(math::getX(v0_rel), math::getX(v1_rel)), math::getX(v2_rel));
	math::real minX = min(min(math::getX(v0_rel), math::getX(v1_rel)), math::getX(v2_rel));

	math::real maxY = max(max(math::getY(v0_rel), math::getY(v1_rel)), math::getY(v2_rel));
	math::real minY = min(min(math::getY(v0_rel), math::getY(v1_rel)), math::getY(v2_rel));

	math::real maxZ = max(max(math::getZ(v0_rel), math::getZ(v1_rel)), math::getZ(v2_rel));
	math::real minZ = min(min(math::getZ(v0_rel), math::getZ(v1_rel)), math::getZ(v2_rel));

	if (maxX < -e0 || minX > e0) return false;
	if (maxY < -e1 || minY > e1) return false;
	if (maxZ < -e2 || minZ > e2) return false;

	math::Vector planeNormal = math::cross(f0, f1);
	math::Vector t = math::dot(planeNormal, v0_rel);
	math::real planeD = math::getScalar(math::dot(planeNormal, v0));
	return checkPlane(leaf, planeNormal, planeD);
}

bool manta::Octree::AABBIntersect(const OctreeBV *leaf, const LightRay *ray, math::real *depth, const math::Vector &ood) const {
	math::real tmin = (math::real)0.0;
	math::real tmax = math::constants::REAL_MAX;

	math::Vector rayDir = ray->getDirection();
	math::Vector rayOrigin = ray->getSource();

	math::Vector maxPoint = leaf->maxPoint; //math::add(math::loadScalar(1E-2), m_maxPoint);
	math::Vector minPoint = leaf->minPoint; //math::sub(m_minPoint, math::loadScalar(1E-2));

	math::Vector t1_v = math::mul(math::sub(minPoint, rayOrigin), ood);
	math::Vector t2_v = math::mul(math::sub(maxPoint, rayOrigin), ood);

	math::Vector t1_v_temp = math::componentMin(t1_v, t2_v);
	t2_v = math::componentMax(t1_v, t2_v);

	math::real rayDirX = math::getX(rayDir);
	if (rayDirX < 1E-6 && rayDirX > -1E-6) {
		if (math::getX(rayOrigin) < math::getX(minPoint) || math::getX(rayOrigin) > math::getX(maxPoint)) return false;
	}
	else {
		math::real t1 = math::getX(t1_v_temp);
		math::real t2 = math::getX(t2_v);

		tmin = (tmin > t1) ? tmin : t1;
		tmax = t2;

		if (tmin > tmax) return false;
	}

	math::real rayDirY = math::getY(rayDir);
	if (rayDirY < 1E-6 && rayDirY > -1E-6) {
		if (math::getY(rayOrigin) < math::getY(minPoint) || math::getY(rayOrigin) > math::getY(maxPoint)) return false;
	}
	else {
		math::real t1 = math::getY(t1_v_temp);
		math::real t2 = math::getY(t2_v);

		tmin = (tmin > t1) ? tmin : t1;
		tmax = (tmax > t2) ? t2 : tmax;

		if (tmin > tmax) return false;
	}

	math::real rayDirZ = math::getZ(rayDir);
	if (rayDirZ < 1E-6 && rayDirZ > -1E-6) {
		if (math::getZ(rayOrigin) < math::getZ(minPoint) || math::getZ(rayOrigin) > math::getZ(maxPoint)) return false;
	}
	else {
		math::real t1 = math::getZ(t1_v_temp);
		math::real t2 = math::getZ(t2_v);

		tmin = (tmin > t1) ? tmin : t1;
		tmax = (tmax > t2) ? t2 : tmax;

		if (tmin > tmax) return false;
	}

	*depth = tmin;
	return true;
}

bool manta::Octree::AABBIntersect(const OctreeBV *leaf, const math::Vector &p, math::real radius) const {
	math::Vector v = p;
	v = math::componentMax(v, leaf->minPoint);
	v = math::componentMin(v, leaf->maxPoint);

	math::real radius2 = radius * radius;
	if (math::getScalar(math::magnitudeSquared3(math::sub(p, v))) > radius2) {
		return false;
	}
	return true;
}
