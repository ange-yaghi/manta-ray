#include <mesh.h>

#include <light_ray.h>
#include <intersection_point.h>

manta::Mesh::Mesh() {
	m_faces = nullptr;
	m_precomputedValues = nullptr;
	m_vertices = nullptr;

	m_fastIntersectEnabled = false;
	m_fastIntersectRadius = (math::real)0.0;
}

manta::Mesh::~Mesh() {
	delete[] m_faces;
	delete[] m_precomputedValues;
	delete[] m_vertices;
}

void manta::Mesh::initialize(int faceCount, int vertexCount) {
	m_faces = new Face[faceCount];
	m_vertices = new SimpleVertex[vertexCount];

	m_faceCount = faceCount;
	m_vertexCount = vertexCount;
}

void manta::Mesh::fixNormals() {
	for (int i = 0; i < m_faceCount; i++) {
		math::Vector u = m_vertices[m_faces[i].u].location;
		math::Vector v = m_vertices[m_faces[i].v].location;
		math::Vector w = m_vertices[m_faces[i].w].location;
		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));
	}
}

void manta::Mesh::precomputeValues() {
	m_precomputedValues = new PrecomputedValues[m_faceCount];

	for (int i = 0; i < m_faceCount; i++) {
		PrecomputedValues *cache = &m_precomputedValues[i];
		math::Vector u = m_vertices[m_faces[i].u].location;
		math::Vector v = m_vertices[m_faces[i].v].location;
		math::Vector w = m_vertices[m_faces[i].w].location;

		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));
		//computePlane(normal, u, &cache->plane);
		cache->normal = math::normalize(normal);

		computePlane(math::cross(normal, math::sub(w, v)), v, &cache->edgePlaneVW);
		computePlane(math::cross(normal, math::sub(u, w)), w, &cache->edgePlaneWU);

		// Scale the planes such that the computed barycentric coordinates are correct
		math::Vector scaleVW = math::loadScalar(
			math::getScalar(math::dot(u, cache->edgePlaneVW.normal)) - cache->edgePlaneVW.d);
		math::Vector scaleWU = math::loadScalar(
			math::getScalar(math::dot(v, cache->edgePlaneWU.normal)) - cache->edgePlaneWU.d);

		cache->edgePlaneVW.normal = math::div(cache->edgePlaneVW.normal, scaleVW);
		cache->edgePlaneVW.d /= math::getScalar(scaleVW);

		cache->edgePlaneWU.normal = math::div(cache->edgePlaneWU.normal, scaleWU);
		cache->edgePlaneWU.d /= math::getScalar(scaleWU);

		cache->p0 = u;
	}
}

void manta::Mesh::detectIntersection(const LightRay *ray, IntersectionPoint *p) const {
	p->m_intersection = false;
	math::real closestDepth = (math::real)DBL_MAX;
	math::Vector rayDir = ray->getDirection();
	math::Vector raySource = ray->getSource();

	for (int i = 0; i < m_faceCount; i++) {
		if (detectIntersection(i, closestDepth, rayDir, raySource, p)) {
			closestDepth = p->m_depth;
		}
	}
}

bool manta::Mesh::fastIntersection(const LightRay *ray) const {
	if (m_fastIntersectEnabled) {
		math::Vector d_pos = math::sub(ray->getSource(), m_position);
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

bool manta::Mesh::detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, IntersectionPoint *p) const {
	PrecomputedValues &cache = m_precomputedValues[faceIndex];

	math::Vector denom = math::dot(cache.normal, rayDir);

	// The ray is nearly perpendicular to the plane
	math::real denom_s = math::getScalar(denom);
	if (denom_s < 1e-6 && denom_s > -1e-6) return false;

	math::Vector p0r0 = math::sub(cache.p0, rayOrigin);
	math::Vector depth = math::div(math::dot(p0r0, cache.normal), denom);

	math::real depth_s = math::getScalar(depth);

	constexpr math::real bias = 0;

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
	// TODO: only apply this logic for two-sided objects
	if (math::getScalar(math::dot(cache.normal, rayDir)) > (math::real)0.0)
		p->m_normal = math::negate(cache.normal);
	else
		p->m_normal = cache.normal; // TODO: calculate smoothed normals based on vertices
	p->m_position = s;

	return true;
}

void manta::Mesh::computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const {
	plane->normal = math::normalize(n);
	plane->d = math::getScalar(math::dot(plane->normal, p));
}
