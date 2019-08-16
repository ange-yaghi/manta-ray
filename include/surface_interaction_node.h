#ifndef MANTARAY_SURFACE_INTERACTION_NODE_H
#define MANTARAY_SURFACE_INTERACTION_NODE_H

#include "node.h"

#include "surface_interaction_node_output.h"

namespace manta {

    class SurfaceInteractionNode : public Node {
    public:
        SurfaceInteractionNode();
        virtual ~SurfaceInteractionNode();

    protected:
        virtual void _initialize();

        SurfaceInteractionNodeOutput<NORMAL> m_normalOutput;
        SurfaceInteractionNodeOutput<FACE_NORMAL> m_faceNormalOutput;
        SurfaceInteractionNodeOutput<POSITION> m_positionOutput;
        SurfaceInteractionNodeOutput<TEXTURE_COORDINATES> m_textureCoordinatesOutputs;
        SurfaceInteractionNodeOutput<DEPTH> m_depthOutput;
        SurfaceInteractionNodeOutput<INCIDENT_DIRECTION> m_incidentDirectionOutput;
        SurfaceInteractionNodeOutput<INCIDENT_SOURCE> m_incidentSourceOutput;

        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_SURFACE_INTERACTION_NODE_H */
