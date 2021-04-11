#ifndef MANTARAY_BUMP_NODE_OUTPUT_H
#define MANTARAY_BUMP_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

    class BumpNodeOutput : public VectorNodeOutput {
    public:
        BumpNodeOutput();
        virtual ~BumpNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        piranha::pNodeInput *getDisplacementConnection() { return &m_displacement; }
        piranha::pNodeInput *getNormalConnection() { return &m_normal; }

        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_displacement;
        piranha::pNodeInput m_normal;
    };

} /* namespace manta */

#endif /* MANTARAY_BUMP_NODE_OUTPUT_H */
