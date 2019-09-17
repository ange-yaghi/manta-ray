#ifndef MANTARAY_INTERSECTION_POINT_H
#define MANTARAY_INTERSECTION_POINT_H

#include "manta_math.h"
#include "media_interface.h"
#include "intersection_point_types.h"

namespace manta {

    class LightRay;
    class IntersectionPointManager;

    struct IntersectionPoint {
        math::Vector m_position;
        math::Vector m_vertexNormal;
        math::Vector m_faceNormal;
        math::Vector m_textureCoodinates;
        LightRay *m_lightRay = nullptr;
        math::real m_depth;

        int m_material;

        bool m_intersection;
        bool m_valid;

        MediaInterface::DIRECTION m_direction;

        intersection_id m_id;
        int m_threadId;
        IntersectionPointManager *m_manager;

    public:
        void calculateCachedValues();

    protected:
        math::Vector m_vertexU;
        math::Vector m_vertexV;

    public:
        math::Vector worldToLocal(const math::Vector &i) const;
        bool isReflection(const math::Vector &i, const math::Vector &o) const;
    };

} /* namespace manta */

#endif /* MANTARAY_INTERSECTION_POINT_H */
