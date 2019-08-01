#ifndef MANTARAY_CONVOLUTION_NODE_H
#define MANTARAY_CONVOLUTION_NODE_H

#include <piranha.h>

#include "vector_map_2d.h"
#include "vector_map_2d_node_output.h"

namespace manta {

    class ConvolutionNode : public piranha::Node{
    public:
        ConvolutionNode();
        ~ConvolutionNode();

        void setInputs(piranha::NodeOutput *base, piranha::NodeOutput *filter) { m_base = base; m_filter = filter; }

        void setResize(bool resize) { m_resize = resize; }
        bool getResize() const { return m_resize; }

        void setClip(bool clip) { m_clip = clip; }
        bool getClip() const { return m_clip; }

        VectorMap2DNodeOutput *getMainOutput() { return &m_output; }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

    protected:
        piranha::NodeOutput *m_base;
        piranha::NodeOutput *m_filter;
        VectorMap2DNodeOutput m_output;
        bool m_resize;
        bool m_clip;

        VectorMap2D m_outputMap;
    };

} /* namespace manta */

#endif /* MANTARAY_CONVOLUTION_NODE_H */

