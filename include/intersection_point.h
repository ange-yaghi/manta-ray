#ifndef INTERSECTION_POINT_H
#define INTERSECTION_POINT_H

#include <manta_math.h>

namespace manta {

	struct IntersectionPoint {
	public:
		math::Vector m_position;
		math::Vector m_vertexNormal;
		math::Vector m_faceNormal;
		math::Vector m_textureCoodinates;
		math::real m_depth;

		int m_material;

		bool m_intersection;
	};

} /* namespace manta */

#endif /* INTERSECTION_POINT_H */
