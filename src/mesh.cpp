#include <mesh.h>

#include <light_ray.h>
#include <intersection_point.h>
#include <obj_file_loader.h>
#include <intersection_list.h>

manta::Mesh::Mesh() {
	m_faces = nullptr;
	m_precomputedValues = nullptr;
	m_vertices = nullptr;
	m_normals = nullptr;

	m_fastIntersectEnabled = false;
	m_fastIntersectRadius = (math::real)0.0;

	m_perVertexNormals = false;
}

manta::Mesh::~Mesh() {
	delete[] m_faces;
	delete[] m_precomputedValues;
	delete[] m_vertices;
	delete[] m_normals;
}

void manta::Mesh::initialize(int faceCount, int vertexCount, int normalCount) {
	m_faces = new Face[faceCount];
	m_vertices = new math::Vector[vertexCount];

	if (normalCount > 0) {
		m_normals = new math::Vector[normalCount];
	}

	m_faceCount = faceCount;
	m_vertexCount = vertexCount;
	m_normalCount = normalCount;
}

void manta::Mesh::precomputeValues() {
	m_precomputedValues = new PrecomputedValues[m_faceCount];

	for (int i = 0; i < m_faceCount; i++) {
		PrecomputedValues *cache = &m_precomputedValues[i];
		math::Vector u = m_vertices[m_faces[i].u];
		math::Vector v = m_vertices[m_faces[i].v];
		math::Vector w = m_vertices[m_faces[i].w];

		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));
		cache->normal = math::normalize(normal);

		computePlane(math::cross(normal, math::sub(w, v)), v, &cache->edgePlaneVW);
		computePlane(math::cross(normal, math::sub(u, w)), w, &cache->edgePlaneWU);
		computePlane(math::cross(normal, math::sub(v, u)), u, &cache->edgePlaneVU);

		// Scale the planes such that the computed barycentric coordinates are correct
		math::Vector scaleVW = math::loadScalar(
			math::getScalar(math::dot(u, cache->edgePlaneVW.normal)) - cache->edgePlaneVW.d);
		math::Vector scaleWU = math::loadScalar(
			math::getScalar(math::dot(v, cache->edgePlaneWU.normal)) - cache->edgePlaneWU.d);

		cache->edgePlaneVW.normal = math::div(cache->edgePlaneVW.normal, scaleVW);
		cache->edgePlaneVW.d /= math::getScalar(scaleVW);

		cache->edgePlaneWU.normal = math::div(cache->edgePlaneWU.normal, scaleWU);
		cache->edgePlaneWU.d /= math::getScalar(scaleWU);

		cache->scaleWU = (math::real)1.0 / math::getScalar(scaleWU);
		cache->scaleVW = (math::real)1.0 / math::getScalar(scaleVW);

		cache->p0 = u;
	}
}

const manta::CoarseIntersection *manta::Mesh::coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, const CoarseIntersection *reference, math::real epsilon) const {
	const manta::CoarseIntersection *closest = reference;
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	CoarseCollisionOutput output;

	for (int i = 0; i < m_faceCount; i++) {
		math::real depthHint = math::constants::REAL_MAX;
		if (closest != nullptr) {
			depthHint = closest->depth + epsilon;
		}

		if (detectIntersection(i, depthHint, rayDir, raySource, 1E-2, &output)) {
			CoarseIntersection *intersection = l->newIntersection();
			intersection->depth = output.depth;
			intersection->locationHint = i; // Face index
			intersection->sceneObject = object;
			intersection->sceneGeometry = this;

			if (closest == nullptr || output.depth < closest->depth) {
				// Check that the collision is eligible to be a reference
				//if (detectIntersection(i, output.u, output.v, output.w, 1E-6)) {
				IntersectionPoint p;
				if (detectIntersection(i, math::constants::REAL_MAX, rayDir, raySource, &p, 1E-6)) {
					//	detectIntersection(i, output.u, output.v, output.w, 1E-6);
					//	detectIntersection(i, math::REAL_MAX, rayDir, raySource, &p, 1E-6);
					//}
					closest = intersection;
				}
				//}
			}
		}
	}

	return closest;
}

void manta::Mesh::fineIntersection(const LightRay *ray, IntersectionPoint *p, const CoarseIntersection *hint, math::real bleed) const {
	p->m_intersection = false;
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	detectIntersection(hint->locationHint, math::constants::REAL_MAX, rayDir, raySource, p, bleed);
}

bool manta::Mesh::fastIntersection(const LightRay *ray) const {
	if (m_fastIntersectEnabled) {
		math::Vector d_pos = math::sub(ray->getSource(), m_fastIntersectPosition);
		math::Vector d_dot_dir = math::dot(d_pos, ray->getDirection());
		math::Vector mag2 = math::magnitudeSquared3(d_pos);

		math::Vector radius2 = math::loadScalar(m_fastIntersectRadius * m_fastIntersectRadius);
		math::Vector det = math::sub(math::mul(d_dot_dir, d_dot_dir), math::sub(mag2, radius2));

		if (math::getScalar(det) < (math::real)0.0) {
			return false;
		}
		else {
			det = math::sqrt(det);
			math::Vector t1 = math::sub(det, d_dot_dir);
			math::Vector t2 = math::sub(math::negate(det), d_dot_dir);

			math::real t1_s = math::getScalar(t1);
			math::real t2_s = math::getScalar(t2);

			return t1_s > (math::real)0.0 || t2_s > (math::real)0.0;
		}
	}
	else return true;
}

void manta::Mesh::loadObjFileData(ObjFileLoader *data) {
	initialize(data->getFaceCount(), data->getVertexCount(), data->getNormalCount());

	for (unsigned int i = 0; i < data->getFaceCount(); i++) {
		ObjFace *face = data->getFace(i);
		m_faces[i].u = face->v1 - 1;
		m_faces[i].v = face->v2 - 1;
		m_faces[i].w = face->v3 - 1;

		m_faces[i].nu = face->vn1 - 1;
		m_faces[i].nv = face->vn2 - 1;
		m_faces[i].nw = face->vn3 - 1;
	}

	for (unsigned int i = 0; i < data->getVertexCount(); i++) {
		math::Vector3 *v = data->getVertex(i);
		m_vertices[i] = math::loadVector(*v);
	}

	if (data->getNormalCount() > 0) {
		for (unsigned int i = 0; i < data->getNormalCount(); i++) {
			math::Vector3 *n = data->getNormal(i);
			m_normals[i] = math::loadVector(*n);
		}
		m_perVertexNormals = true;
	}
	else {
		m_perVertexNormals = false;
	}
	precomputeValues();
}

bool manta::Mesh::detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, IntersectionPoint *p, math::real bleed) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector denom = math::dot(cache.normal, rayDir);

	// The ray is nearly perpendicular to the plane
	math::real denom_s = math::getScalar(denom);
	if (denom_s < 1e-6 && denom_s > -1e-6) return false;

	math::Vector p0r0 = math::sub(cache.p0, rayOrigin);
	math::Vector depth = math::div(math::dot(p0r0, cache.normal), denom);

	math::real depth_s = math::getScalar(depth);

	const math::real bias = -bleed;

	// This ray either does not intersect the plane or intersects at a depth that is further than the early exit hint
	if (depth_s <= (math::real)0.0 || depth_s > earlyExitDepthHint) return false;
	
	math::Vector s = math::add(rayOrigin, math::mul(rayDir, depth));
	
	// Compute barycentric components u, v, w
	math::real u = math::getScalar(math::dot(s, cache.edgePlaneVW.normal)) - cache.edgePlaneVW.d;
	if (u < bias || u > (math::real)1.0-bias) return false;

	math::real v = math::getScalar(math::dot(s, cache.edgePlaneWU.normal)) - cache.edgePlaneWU.d;
	if (v < bias) return false;

	math::real w = (math::real)1.0 - u - v;
	if (w < bias) return false;

	p->m_depth = depth_s;
	p->m_intersection = true;

	math::Vector vertexNormal;
	
	if (m_perVertexNormals) {
		math::Vector normalU = m_normals[m_faces[faceIndex].nu];
		math::Vector normalV = m_normals[m_faces[faceIndex].nv];
		math::Vector normalW = m_normals[m_faces[faceIndex].nw];

		vertexNormal = math::add(math::mul(normalU, math::loadScalar(u)), math::mul(normalV, math::loadScalar(v)));
		vertexNormal = math::add(vertexNormal, math::mul(normalW, math::loadScalar(w)));
		vertexNormal = math::normalize(vertexNormal);
	}
	else {
		vertexNormal = cache.normal;
	}

	// TODO: only apply this logic for two-sided objects
	if (math::getScalar(math::dot(cache.normal, rayDir)) > (math::real)0.0) {
		p->m_vertexNormal = math::negate(vertexNormal);
		p->m_faceNormal = math::negate(cache.normal);
	}
	else {
		p->m_vertexNormal = vertexNormal;
		p->m_faceNormal = cache.normal;
	}
	p->m_position = s;

	return true;
}

bool manta::Mesh::detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, math::real delta, CoarseCollisionOutput *output) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector denom = math::dot(cache.normal, rayDir);

	// The ray is nearly perpendicular to the plane
	math::real denom_s = math::getScalar(denom);
	if (denom_s < 1e-6 && denom_s > -1e-6) return false;

	math::Vector p0r0 = math::sub(cache.p0, rayOrigin);
	math::Vector d = math::div(math::dot(p0r0, cache.normal), denom);

	math::real depth_s = math::getScalar(d);

	// This ray either does not intersect the plane or intersects at a depth that is further than the early exit hint
	if (depth_s <= (math::real)0.0 || depth_s > earlyExitDepthHint) return false;

	math::Vector s = math::add(rayOrigin, math::mul(rayDir, d));

	// Compute quasi-barycentric components u, v, w
	math::real u = math::getScalar(math::dot(s, cache.edgePlaneVW.normal));
	if (u < (cache.edgePlaneVW.d - delta * cache.scaleVW)) return false;

	math::real v = math::getScalar(math::dot(s, cache.edgePlaneWU.normal));
	if (v < (cache.edgePlaneWU.d - delta * cache.scaleWU)) return false;

	math::real w = math::getScalar(math::dot(s, cache.edgePlaneVU.normal));
	if (w < (cache.edgePlaneVU.d - delta)) return false;

	output->depth = depth_s;
	output->u = u;
	output->v = v;
	output->w = w;

	return true;
}

inline bool manta::Mesh::detectIntersection(int faceIndex, math::real u, math::real v, math::real w, math::real delta) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::real ru = u - cache.edgePlaneVW.d;
	math::real rv = v - cache.edgePlaneWU.d;

	// Check quasi-barycentric components u, v, w
	if (ru < -delta || ru > ((math::real)1.0 + delta)) return false;
	if (rv < -delta) return false;
	if (1 - ru - rv < -delta) return false;

	return true;
}

void manta::Mesh::computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const {
	plane->normal = math::normalize(n);
	plane->d = math::getScalar(math::dot(plane->normal, p));
}
