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

			math::real t1, t2;

			t1 = math::getX(t1_v_temp);
			t2 = math::getX(t2_v);

			tmin = (tmin < t1) ? t1 : tmin;
			tmax = t2;

			if (tmin > tmax) return false;

			t1 = math::getY(t1_v_temp);
			t2 = math::getY(t2_v);

			tmin = (tmin < t1) ? t1 : tmin;
			tmax = (tmax > t2) ? t2 : tmax;

			if (tmin > tmax) return false;

			t1 = math::getZ(t1_v_temp);
			t2 = math::getZ(t2_v);

			tmin = (tmin < t1) ? t1 : tmin;
			tmax = (tmax > t2) ? t2 : tmax;

			if (tmin > tmax) return false;

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
