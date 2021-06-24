#ifndef MANTARAY_CONSTRUCTED_COMPLEX_NODE_OUTPUT_H
#define MANTARAY_CONSTRUCTED_COMPLEX_NODE_OUTPUT_H

#include "complex_node_output.h"

#include "intersection_point.h"

namespace manta {

    class ConstructedComplexNodeOutput : public ComplexNodeOutput {
    public:
        ConstructedComplexNodeOutput();
        virtual ~ConstructedComplexNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        piranha::pNodeInput *getRConnection() { return &m_r; }
        piranha::pNodeInput *getIConnection() { return &m_i; }

        void registerInputs();

        virtual void _evaluateDimensions();

    protected:
        piranha::pNodeInput m_r;
        piranha::pNodeInput m_i;
    };

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_COMPLEX_NODE_OUTPUT_H */
