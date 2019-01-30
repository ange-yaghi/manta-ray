#include <sphere_primitive.h>

#include <light_ray.h>
#include <intersection_point.h>
#include <coarse_intersection.h>

manta::SpherePrimitive::SpherePrimitive() {
	m_radius = (math::real)0.0;
}

manta::SpherePrimitive::~SpherePrimitive() {

}

bool manta::SpherePrimitive::fastIntersection(const LightRay *ray) const {
	return true;
}

bool manta::SpherePrimitive::findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const {	
	math::Vector d_pos = math::sub(ray->getSource(), m_position);
	math::Vector d_dot_dir = math::dot(d_pos, ray->getDirection());
	math::Vector mag2 = math::magnitudeSquared3(d_pos);

	math::Vector radius2 = math::loadScalar(m_radius * m_radius);
	math::Vector det = math::sub(math::mul(d_dot_dir, d_dot_dir), math::sub(mag2, radius2));

	if (math::getScalar(det) < (math::real)0.0) {
		// No intersection detected
		return false;
	}
	else {
		det = math::sqrt(det);
		math::Vector t1 = math::sub(det, d_dot_dir);
		math::Vector t2 = math::sub(math::negate(det), d_dot_dir);

		math::real t1_s = math::getScalar(t1);
		math::real t2_s = math::getScalar(t2);

		bool intersects = t1_s > (math::real)0.0 || t2_s > (math::real)0.0;

		if (intersects) {
			if ((t1_s < maxDepth && t1_s > minDepth) || (t2_s < maxDepth && t2_s > minDepth)) {
				intersection->locationHint = -1; // Unused for spheres
				intersection->sceneGeometry = this;
				intersection->globalHint = -1;
				intersection->valid = true;

				math::Vector t;
				if (t2_s < t1_s && t2_s > minDepth) {
					t = t2;
					intersection->depth = t2_s;
				}
				else {
					t = t1;
					intersection->depth = t1_s;
				}

				return true;
			}
			else return false;
		}
		else return false;
	}
}

manta::math::Vector manta::SpherePrimitive::getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const {
	math::Vector p_p0 = math::sub(p, m_position);
	math::Vector closestPoint = math::add(math::mul(math::normalize(p_p0), math::loadScalar(m_radius)), m_position);
	return closestPoint;
}

void manta::SpherePrimitive::getVicinity(const math::Vector &p, math::real radius, IntersectionList *list, SceneObject *object) const {
	/* Deprecated */
}

void manta::SpherePrimitive::fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const {
	math::Vector position = getClosestPoint(hint, r);

	p->m_position = r;

	// Calculate the normal
	math::Vector normal = math::sub(p->m_position, m_position);
	normal = math::normalize(normal);

	p->m_depth = hint->depth;
	p->m_vertexNormal = normal;
	p->m_faceNormal = normal;
	p->m_textureCoodinates = math::constants::Zero;
	p->m_material = -1;
	p->m_valid = true;
	p->m_intersection = true;
}

void manta::SpherePrimitive::detectIntersection(const LightRay *ray, IntersectionPoint *convex, IntersectionPoint *concave) const {
	math::Vector d_pos = math::sub(ray->getSource(), m_position);
	math::Vector d_dot_dir = math::dot(d_pos, ray->getDirection());
	math::Vector mag2 = math::magnitudeSquared3(d_pos);

	math::Vector radius2 = math::loadScalar(m_radius * m_radius);
	math::Vector det = math::sub(math::mul(d_dot_dir, d_dot_dir), math::sub(mag2, radius2));

	if (math::getScalar(det) < (math::real)0.0) {
		if (convex != nullptr) convex->m_intersection = false;
		if (concave != nullptr) concave->m_intersection = false;
	}
	else {
		det = math::sqrt(det);
		math::Vector t1 = math::sub(det, d_dot_dir);
		math::Vector t2 = math::sub(math::negate(det), d_dot_dir);

		math::real t1_s = math::getScalar(t1);
		math::real t2_s = math::getScalar(t2);

		// Initialize these to false
		if (convex != nullptr) convex->m_intersection = false;
		if (concave != nullptr) concave->m_intersection = false;

		if (t1_s > (math::real)0.0) {
			math::Vector pos = math::add(ray->getSource(), math::mul(ray->getDirection(), t1));

			// Calculate the normal
			math::Vector normal = math::sub(pos, m_position);
			normal = math::normalize(normal);

			if (math::getScalar(math::dot(ray->getDirection(), normal)) > 0) {
				if (concave != nullptr) {
					concave->m_intersection = true;
					concave->m_depth = t1_s;
					concave->m_position = pos;
					concave->m_faceNormal = math::negate(normal);
					concave->m_vertexNormal = concave->m_faceNormal;
					concave->m_material = -1;
				}
			}
			else {
				if (convex != nullptr) {
					convex->m_intersection = true;
					convex->m_depth = t1_s;
					convex->m_position = pos;
					convex->m_faceNormal = normal;
					convex->m_vertexNormal = convex->m_faceNormal;
					convex->m_material = -1;
				}
			}
		}
			
		if (t2_s > (math::real)0.0) {
			math::Vector pos = math::add(ray->getSource(), math::mul(ray->getDirection(), t2));

			// Calculate the normal
			math::Vector normal = math::sub(pos, m_position);
			normal = math::normalize(normal);

			if (math::getScalar(math::dot(ray->getDirection(), normal)) > 0) {
				if (concave != nullptr) {
					concave->m_intersection = true;
					concave->m_depth = t1_s;
					concave->m_position = pos;
					concave->m_faceNormal = math::negate(normal);
					concave->m_vertexNormal = concave->m_faceNormal;
					concave->m_material = -1;
				}
			}
			else {
				if (convex != nullptr) {
					convex->m_intersection = true;
					convex->m_depth = t1_s;
					convex->m_position = pos;
					convex->m_faceNormal = normal;
					convex->m_vertexNormal = convex->m_faceNormal;
					convex->m_material = -1;
				}
			}
		}
	}
}
