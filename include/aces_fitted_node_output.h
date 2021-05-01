#ifndef MANTARAY_ACES_FITTED_NODE_OUTPUT_H
#define MANTARAY_ACES_FITTED_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

    class ACESFittedNodeOutput : public VectorNodeOutput {
    public:
        ACESFittedNodeOutput();
        virtual ~ACESFittedNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        piranha::pNodeInput *getColorConnection() { return &m_color; }

        virtual void registerInputs();
        virtual void _evaluateDimensions();

    protected:
        static math::Vector RRTAndODTFit(const math::Vector &v);
        static math::Vector HillACESFitted(const math::Vector &color);

        piranha::pNodeInput m_color;
    };

} /* namespace manta */

#endif /* MANTARAY_ACES_FITTED_NODE_OUTPUT_H */
