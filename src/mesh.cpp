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

void manta::Mesh::precomputeValues() {
	m_precomputedValues = new PrecomputedValues[m_faceCount];

	for (int i = 0; i < m_faceCount; i++) {
		PrecomputedValues *cache = &m_precomputedValues[i];
		math::Vector u = m_vertices[m_faces[i].u].location;
		math::Vector v = m_vertices[m_faces[i].v].location;
		math::Vector w = m_vertices[m_faces[i].w].location;

		math::Vector normal = math::cross(math::sub(v, u), math::sub(w, u));
		computePlane(normal, u, &cache->plane);

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
	}
}

void manta::Mesh::detectIntersection(const LightRay *ray, IntersectionPoint *p) const {
	p->m_intersection = false;
	math::real closestDepth = (math::real)DBL_MAX;
	for (int i = 0; i < m_faceCount; i++) {
		if (detectIntersection(i, closestDepth, ray, p)) {
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

bool manta::Mesh::detectIntersection(int faceIndex, math::real earlyExitDepthHint, const LightRay *ray, IntersectionPoint *p) const {
	Face *face = &m_faces[faceIndex];
	PrecomputedValues *cache = &m_precomputedValues[faceIndex];

	math::Vector denom = math::dot(cache->plane.normal, ray->getDirection());

	// The ray is nearly perpendicular to the plane
	if (abs(math::getScalar(denom)) < 1e-6) return false;

	math::Vector p0r0 = math::sub(m_vertices[face->u].location, ray->getSource());
	math::Vector depth = math::div(math::dot(p0r0, cache->plane.normal), denom);

	math::real depth_s = math::getScalar(depth);

	// This ray either does not intersect the plane or intersects at a depth that is further than the early exit hint
	if (depth_s <= (math::real)0.0 || depth_s > earlyExitDepthHint) return false;
	
	math::Vector s = math::add(ray->getSource(), math::mul(ray->getDirection(), depth));
	
	// Compute barycentric components u, v, w
	math::real u = math::getScalar(math::dot(s, cache->edgePlaneVW.normal)) - cache->edgePlaneVW.d;
	if (u < (math::real)0.0 || u > (math::real)1.0) return false;

	math::real v = math::getScalar(math::dot(s, cache->edgePlaneWU.normal)) - cache->edgePlaneWU.d;
	if (v < (math::real)0.0) return false;

	math::real w = (math::real)1.0 - u - v;
	if (w < (math::real)0.0) return false;

	p->m_depth = depth_s;
	p->m_intersection = true;
	// TODO: only apply this logic for two-sided objects
	if (math::getScalar(math::dot(cache->plane.normal, ray->getDirection())) > (math::real)0.0)
		p->m_normal = math::negate(cache->plane.normal);
	else
		p->m_normal = cache->plane.normal; // TODO: calculate smoothed normals based on vertices
	p->m_position = s;

	return true;
}

void manta::Mesh::computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const {
	plane->normal = math::normalize(n);
	plane->d = math::getScalar(math::dot(plane->normal, p));
}
