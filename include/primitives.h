#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <manta_math.h>

#include <light_ray.h>

namespace manta {

	class LightRay;

	struct AABB {
		math::Vector minPoint;
		math::Vector maxPoint;

		math::real surfaceArea();
		void merge(const AABB &b);
		inline bool rayIntersect(const LightRay &ray, math::real *tmin_out, math::real *tmax_out) const {
			math::real tmin = (math::real)0.0;
			math::real tmax = math::constants::REAL_MAX;

			math::Vector rayDir = ray.getDirection();
			math::Vector rayOrigin = ray.getSource();
			math::Vector ood = ray.getInverseDirection();

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

			*tmin_out = tmin;
			*tmax_out = tmax;
			return true;
		}
	};

	struct Triangle {
		math::Vector u;
		math::Vector v;
		math::Vector w;
	};

	void calculateAABB(const Triangle *triangle, AABB *target);

} /* namespace manta */

#endif /* PRIMITIVES_H */
