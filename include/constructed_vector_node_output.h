#ifndef MANTARAY_CONSTRUCTED_VECTOR_NODE_OUTPUT_H
#define MANTARAY_CONSTRUCTED_VECTOR_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

    class ConstructedVectorNodeOutput : public VectorNodeOutput {
    public:
        ConstructedVectorNodeOutput();
        virtual ~ConstructedVectorNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2D(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        piranha::pNodeInput *getXConnection() { return &m_x; }
        piranha::pNodeInput *getYConnection() { return &m_y; }
        piranha::pNodeInput *getZConnection() { return &m_z; }
        piranha::pNodeInput *getWConnection() { return &m_w; }

        const NodeOutput *getXOutput() { return m_x; }

        void registerInputs();

    protected:
        piranha::pNodeInput m_x;
        piranha::pNodeInput m_y;
        piranha::pNodeInput m_z;
        piranha::pNodeInput m_w;
    };

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_VECTOR_NODE_OUTPUT_H */