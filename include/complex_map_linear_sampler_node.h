#ifndef MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_NODE_H
#define MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_NODE_H

#include "object_reference_node.h"

#include "complex_map_linear_sampler.h"

namespace manta {

    class ComplexMapLinearSamplerNode : public ObjectReferenceNode<ComplexMapSampler> {
    public:
        ComplexMapLinearSamplerNode() {
            /* void */
        }

        virtual ~ComplexMapLinearSamplerNode() {
            /* void */
        }

    protected:
        virtual void _evaluate() {
            setOutput(&m_sampler);
        }

    protected:
        ComplexMapLinearSampler m_sampler;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_NODE_H */
