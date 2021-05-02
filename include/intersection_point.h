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
        math::Vector m_inside;
        math::Vector m_outside;
        math::Vector m_vertexNormal;
        math::Vector m_faceNormal;
        math::Vector m_textureCoodinates;
        LightRay *m_lightRay = nullptr;
        math::real m_depth;

        int m_material;

        bool m_intersection;
        bool m_valid;

        MediaInterface::Direction m_direction;

        intersection_id m_id = 0;
        int m_threadId = 0;
        IntersectionPointManager *m_manager;

        Light *m_light = nullptr;
        const Mesh *m_mesh = nullptr;
        const BSDF *m_bsdf = nullptr;
        int m_faceIndex = -1;

    public:
        math::Vector u_basis;
        math::Vector v_basis;

        math::Vector p0;
        math::Vector n0;
        math::Vector t0;

        math::real u;
        math::real v;

        math::Vector dtdu;
        math::Vector dtdv;
        math::Vector dndu;
        math::Vector dndv;

        void calculatePartialDerivatives();
        void offset(math::real du, math::real dv);

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
