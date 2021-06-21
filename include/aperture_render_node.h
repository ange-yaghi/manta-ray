#ifndef MANTARAY_APERTURE_RENDER_NODE_H
#define MANTARAY_APERTURE_RENDER_NODE_H

#include "node.h"

#include "vector_map_2d_node_output.h"

namespace manta {

    class ApertureRenderNode : public Node {
    public:
        ApertureRenderNode();
        ~ApertureRenderNode();

        VectorMap2DNodeOutput *getMainOutput() { return &m_output; }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::pNodeInput m_aperture;
        piranha::pNodeInput m_imagePlane;
        piranha::pNodeInput m_scale;
        piranha::pNodeInput m_multisample;

        VectorMap2DNodeOutput m_output;

    protected:
        VectorMap2D *m_outputMap;
    };

} /* namespace manta */

#endif /* MANTARAY_APERTURE_RENDER_NODE_H */
