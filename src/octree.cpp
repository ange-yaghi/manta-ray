#include <octree.h>

#include <scene_object.h>
#include <mesh.h>
#include <light_ray.h>
#include <intersection_list.h>

#include <assert.h>
#include <math.h>

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

const manta::CoarseIntersection *manta::Octree::coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, const CoarseIntersection *reference, math::real epsilon) const {
	math::real depth;
	const CoarseIntersection *closest = reference;
	if (AABBIntersect(ray, &depth)) {
		if (m_mesh != nullptr) {
			closest = m_mesh->coarseIntersection(ray, l, object, closest, epsilon);
		}

		if (m_children != nullptr) {
			for (int i = 0; i < 8; i++) {
				closest = m_children[i].coarseIntersection(ray, l, object, closest, epsilon);
			}
		}
	}
	return closest;
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
	int internalFaceCount = (int)m_tempFaces.size();
	int realVertexCount = 0;
	int realFaceCount = 0;

	if (getUsageInternal() > 0) {
		vertexMap = new int[mesh->getVertexCount()];
		for (int i = 0; i < mesh->getVertexCount(); i++) vertexMap[i] = -1;

		for (int i = 0; i < internalFaceCount; i++) {
			if (!m_tempFaces[i].presentInChild) {
				Face *face = &mesh->getFaces()[m_tempFaces[i].face];
				int u = face->u;
				int v = face->v;
				int w = face->w;

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
		m_mesh->initialize(realFaceCount, realVertexCount, realVertexCount);

		math::Vector *newVertices = m_mesh->getVertices();
		Face *newFaces = m_mesh->getFaces();

		int currentFace = 0;
		for (int i = 0; i < internalFaceCount; i++) {
			if (!m_tempFaces[i].presentInChild) {
				Face *face = &mesh->getFaces()[m_tempFaces[i].face];
				int u = face->u;
				int v = face->v;
				int w = face->w;

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
			if (math::get(rayOrigin, i) < math::get(m_minPoint, i) || math::get(rayOrigin, i) < math::get(m_minPoint, i)) return false;
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
