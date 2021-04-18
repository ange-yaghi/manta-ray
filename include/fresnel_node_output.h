#ifndef MANTARAY_FRESNEL_NODE_OUTPUT_H
#define MANTARAY_FRESNEL_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

    class FresnelNodeOutput : public VectorNodeOutput {
    public:
        FresnelNodeOutput();
        virtual ~FresnelNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        piranha::pNodeInput *getIorConnection() { return &m_iorInput; }
        piranha::pNodeInput *getNormalConnection() { return &m_normalInput; }

        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_iorInput;
        piranha::pNodeInput m_normalInput;
    };

} /* namespace manta */

#endif /* MANTARAY_FRESNEL_NODE_OUTPUT_H */
