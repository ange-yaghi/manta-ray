#ifndef MANTARAY_SURFACE_INTERACTION_NODE_OUTPUT_H
#define MANTARAY_SURFACE_INTERACTION_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"
#include "light_ray.h"

namespace manta {

    enum SURFACE_INTERACTION_PROPERTY {
        NORMAL,
        FACE_NORMAL,
        POSITION,
        TEXTURE_COORDINATES,
        DEPTH,
        INCIDENT_DIRECTION,
        INCIDENT_SOURCE
    };

    template <SURFACE_INTERACTION_PROPERTY op>
    class SurfaceInteractionNodeOutput : public VectorNodeOutput {
    public:

    public:
        SurfaceInteractionNodeOutput() {
            /* void */
        }

        virtual ~SurfaceInteractionNodeOutput() {
            /* void */
        }

        static inline math::Vector doOp(const IntersectionPoint *surfaceInteraction) {
            switch (op) {
            case NORMAL:
                return surfaceInteraction->m_vertexNormal;
            case FACE_NORMAL:
                return surfaceInteraction->m_faceNormal;
            case POSITION:
                return surfaceInteraction->m_position;
            case TEXTURE_COORDINATES:
                return surfaceInteraction->m_textureCoodinates;
            case DEPTH:
                return math::loadScalar(surfaceInteraction->m_depth);
            case INCIDENT_DIRECTION:
                return (surfaceInteraction->m_lightRay != nullptr) 
                    ? surfaceInteraction->m_lightRay->getDirection() 
                    : math::constants::Zero;
            case INCIDENT_SOURCE:
                return (surfaceInteraction->m_lightRay != nullptr)
                    ? surfaceInteraction->m_lightRay->getSource()
                    : math::constants::Zero;
            default:
                return math::constants::Zero;
            }
        }

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
            math::Vector *target = reinterpret_cast<math::Vector *>(_target);
            *target = (surfaceInteraction != nullptr)
                ? doOp(surfaceInteraction)
                : math::constants::Zero;
        }

        virtual void discreteSample2d(int x, int y, void *_target) const {
            math::Vector *target = reinterpret_cast<math::Vector *>(_target);
            *target = math::constants::Zero;
        }

        virtual void getDataReference(const void **target) const {
            *target = nullptr;
        }
    };

} /* namespace manta */

#endif /* MANTARAY_SURFACE_INTERACTION_NODE_OUTPUT_H */

