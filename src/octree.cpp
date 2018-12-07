#include <octree.h>

#include <scene_object.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_list.h>
#include <stack_list.h>

#include <assert.h>
#include <math.h>
#include <fstream>

manta::Octree::Octree() {
	m_children = nullptr;
	m_mesh = nullptr;
}

manta::Octree::~Octree() {
	assert(m_children == nullptr);
}


void manta::Octree::initialize(math::real width, const math::Vector &position) {
	m_width = width;
	m_position = position;

	math::real epsWidth = (math::real)(1.0 + 1E-4) * m_width;

	m_maxPoint = math::add(m_position, math::loadVector(epsWidth, epsWidth, epsWidth));
	m_minPoint = math::sub(m_position, math::loadVector(epsWidth, epsWidth, epsWidth));
}

void manta::Octree::destroy() {
	if (m_children != nullptr) {
		for (int i = 0; i < 8; i++) {
			m_children[i].destroy();
		}
		delete[] m_children;
		m_children = nullptr;
	}
}

const manta::CoarseIntersection *manta::Octree::coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, const CoarseIntersection *reference, math::real epsilon, StackAllocator *s) const {
	math::real depth;
	const CoarseIntersection *closest = reference;
	if ((m_mesh != nullptr || m_children != nullptr) && AABBIntersect(ray, &depth)) {
		if (closest != nullptr && depth > closest->depth + epsilon) return closest;

		if (m_mesh != nullptr) {
			closest = m_mesh->coarseIntersection(ray, l, object, closest, epsilon, s);
		}

		if (m_children != nullptr) {
			for (int i = 0; i < 8; i++) {
				closest = m_children[i].coarseIntersection(ray, l, object, closest, epsilon, s);
			}
		}
	}
	return closest;
	
	/*
	const CoarseIntersection *closest = reference;

	StackList<OctreeLeafCollision> intersectionList;
	intersectionList.setStack(nullptr);

	// Find all leaves intersected by the ray
	octreeTest(ray, &intersectionList);

	// Sort the leaves by depth 
	// Simple insertion sort for now
	
	for (int i = 0; i < intersectionList.getItemCount(); i++) {
		int j = i;
		while (j > 0 && intersectionList.getItem(j - 1)->depth > intersectionList.getItem(j)->depth) {
			intersectionList.swapElements(j, j - 1);
			j--;
		}
	}

	OctreeLeafCollision *buffer = new OctreeLeafCollision[intersectionList.getItemCount()];
	int leafCount = intersectionList.getItemCount();
	intersectionList.fillBuffer(buffer);
	intersectionList.destroy();

	int faceCount = 0;
	int leaves = 0;

	for (int i = 0; i < leafCount; i++) {
		OctreeLeafCollision *leafColl = &buffer[i];
		// Early exit if the bounding box is beyond the current point
		if (closest != nullptr && leafColl->depth > closest->depth + epsilon) break;
		closest = leafColl->leaf->m_mesh->coarseIntersection(ray, l, object, closest, epsilon, s);
		faceCount += leafColl->leaf->m_mesh->getFaceCount();
		leaves++;
	}

	delete[] buffer;

	return closest;*/
}

void manta::Octree::fineIntersection(const LightRay *ray, IntersectionPoint *p, const CoarseIntersection *hint, math::real bleed) const {
	hint->sceneGeometry->fineIntersection(ray, p, hint, bleed);
}

bool manta::Octree::fastIntersection(const LightRay * ray) const {
	return true;
}

void manta::Octree::analyze(SceneObject *object, int maxSize) {
	analyze(object, nullptr, maxSize);
}

void manta::Octree::writeToObjFile(const char *fname, const LightRay *ray) const {
	std::ofstream f(fname);

	assert(f.is_open());

	int currentLeaf = 0;
	writeToObjFile(f, currentLeaf, ray);

	f.close();
}

void manta::Octree::writeToObjFile(std::ofstream &f, int &currentLeaf, const LightRay *ray) const {
	math::real depth;
	if ((m_mesh != nullptr || m_children != nullptr) && (ray == nullptr || AABBIntersect(ray, &depth))) {
		f << "o " << "LEAF_" << currentLeaf << std::endl;

		int vertexOffset = currentLeaf * 8 + 1;
		f << "v " << math::getX(m_minPoint) << " " << math::getY(m_minPoint) << " " << math::getZ(m_minPoint) << std::endl;
		f << "v " << math::getX(m_maxPoint) << " " << math::getY(m_minPoint) << " " << math::getZ(m_minPoint) << std::endl;
		f << "v " << math::getX(m_minPoint) << " " << math::getY(m_minPoint) << " " << math::getZ(m_maxPoint) << std::endl;
		f << "v " << math::getX(m_maxPoint) << " " << math::getY(m_minPoint) << " " << math::getZ(m_maxPoint) << std::endl;
		f << "v " << math::getX(m_minPoint) << " " << math::getY(m_maxPoint) << " " << math::getZ(m_minPoint) << std::endl;
		f << "v " << math::getX(m_maxPoint) << " " << math::getY(m_maxPoint) << " " << math::getZ(m_minPoint) << std::endl;
		f << "v " << math::getX(m_minPoint) << " " << math::getY(m_maxPoint) << " " << math::getZ(m_maxPoint) << std::endl;
		f << "v " << math::getX(m_maxPoint) << " " << math::getY(m_maxPoint) << " " << math::getZ(m_maxPoint) << std::endl;

		f << "f " << vertexOffset << " " << vertexOffset + 1 << " " << vertexOffset + 3 << " " << vertexOffset + 2 << std::endl;
		f << "f " << vertexOffset << " " << vertexOffset + 4 << " " << vertexOffset + 6 << " " << vertexOffset + 2 << std::endl;
		f << "f " << vertexOffset << " " << vertexOffset + 4 << " " << vertexOffset + 5 << " " << vertexOffset + 1 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 5 << " " << vertexOffset + 1 << " " << vertexOffset + 3 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 6 << " " << vertexOffset + 2 << " " << vertexOffset + 3 << std::endl;
		f << "f " << vertexOffset + 7 << " " << vertexOffset + 6 << " " << vertexOffset + 4 << " " << vertexOffset + 5 << std::endl;

		currentLeaf++;

		if (m_children != nullptr) {
			for (int i = 0; i < 8; i++) {
				m_children[i].writeToObjFile(f, currentLeaf, ray);
			}
		}
	}
}

void manta::Octree::octreeTest(const LightRay *ray, StackList<OctreeLeafCollision> *list) const {
	math::real depth;
	if ((m_mesh != nullptr || m_children != nullptr) && AABBIntersect(ray, &depth)) {
		if (m_mesh != nullptr) {
			OctreeLeafCollision *leafCollision = list->newItem();
			leafCollision->depth = depth;
			leafCollision->leaf = this;
		}

		if (m_children != nullptr) {
			for (int i = 0; i < 8; i++) {
				m_children[i].octreeTest(ray, list);
			}
		}
	}
}

void manta::Octree::analyze(SceneObject *object, Octree *parent, int maxSize) {
	Mesh *mesh = (Mesh *)object->getGeometry();
	Face *faces = mesh->getFaces();

	int faceCount = (parent == nullptr) ? (int)mesh->getFaceCount() : (int)parent->m_tempFaces.size();

	for (int i = 0; i < faceCount; i++) {
		int faceIndex = (parent == nullptr) ? i : parent->m_tempFaces[i].face;

		math::Vector v1 = mesh->getVertices()[faces[faceIndex].u];
		math::Vector v2 = mesh->getVertices()[faces[faceIndex].v];
		math::Vector v3 = mesh->getVertices()[faces[faceIndex].w];

		if (!checkVertex(v1, 1E-5)) continue;
		if (!checkVertex(v2, 1E-5)) continue;
		if (!checkVertex(v3, 1E-5)) continue;

		TempFace f;
		f.face = faceIndex;
		f.presentInChild = false;
		m_tempFaces.push_back(f);
		if (parent != nullptr) {
			parent->m_tempFaces[i].presentInChild = true;
		}
	}

	if (getUsageInternal() > maxSize) {
		m_children = new manta::Octree[8];
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
			m_children[i].analyze(object, this, maxSize);
		}
	}

	int *vertexMap = nullptr;
	int *normalMap = nullptr;
	int internalFaceCount = (int)m_tempFaces.size();
	int realVertexCount = 0;
	int realNormalCount = 0;
	int realFaceCount = 0;

	if (getUsageInternal() > 0) {
		vertexMap = new int[mesh->getVertexCount()];
		if (mesh->getNormalCount() > 0) normalMap = new int[mesh->getNormalCount()];
		for (int i = 0; i < mesh->getVertexCount(); i++) vertexMap[i] = -1;
		for (int i = 0; i < mesh->getNormalCount(); i++) normalMap[i] = -1;

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
		m_mesh = new Mesh;
		m_mesh->initialize(realFaceCount, realVertexCount, realNormalCount);
		m_mesh->setPerVertexNormals(realNormalCount > 0);

		math::Vector *newVertices = m_mesh->getVertices();
		math::Vector *newNormals = m_mesh->getNormals();
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

				newVertices[vertexMap[u]] = mesh->getVertices()[u];
				newVertices[vertexMap[v]] = mesh->getVertices()[v];
				newVertices[vertexMap[w]] = mesh->getVertices()[w];

				newFaces[currentFace].u = vertexMap[u];
				newFaces[currentFace].v = vertexMap[v];
				newFaces[currentFace].w = vertexMap[w];

				currentFace++;
			}
		}
		m_mesh->precomputeValues();
	}

	m_tempFaces.clear();

	if (vertexMap != nullptr) delete[] vertexMap;
	if (normalMap != nullptr) delete[] normalMap;
}

bool manta::Octree::checkVertex(const math::Vector &v, math::real epsilon) const {
	if (math::getX(v) - math::getX(m_maxPoint) > epsilon) {
		return false;
	}
	if (math::getY(v) - math::getY(m_maxPoint) > epsilon) {
		return false;
	}
	if (math::getZ(v) - math::getZ(m_maxPoint) > epsilon) {
		return false;
	}

	if (math::getX(m_minPoint) - math::getX(v) > epsilon) {
		return false;
	}
	if (math::getY(m_minPoint) - math::getY(v) > epsilon) {
		return false;
	}
	if (math::getZ(m_minPoint) - math::getZ(v) > epsilon) {
		return false;
	}
	return true;
}

bool manta::Octree::AABBIntersect(const LightRay *ray, math::real *depth) const {
	math::real tmin = (math::real)0.0;
	math::real tmax = math::constants::REAL_MAX;

	math::Vector rayDir = ray->getDirection();
	math::Vector rayOrigin = ray->getSource();

	for (int i = 0; i < 3; i++) {
		if (abs(math::get(rayDir, i)) < 1E-6) {
			// Ray is parallel
			if (math::get(rayOrigin, i) < math::get(m_minPoint, i) || math::get(rayOrigin, i) > math::get(m_maxPoint, i)) return false;
		}
		else {
			math::real ood = (math::real)1.0 / math::get(rayDir, i);
			math::real t1 = (math::get(m_minPoint, i) - math::get(rayOrigin, i)) * ood;
			math::real t2 = (math::get(m_maxPoint, i) - math::get(rayOrigin, i)) * ood;

			if (t1 > t2) {
				math::real temp = t1;
				t1 = t2;
				t2 = temp;
			}

			tmin = (tmin > t1) ? tmin : t1;
			tmax = (tmax > t2) ? t2 : tmax;

			if (tmin > tmax) return false;
		}
	}
	*depth = tmin;
	return true;
}
