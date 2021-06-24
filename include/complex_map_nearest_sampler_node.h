#ifndef MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_NODE_H
#define MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_NODE_H

#include "object_reference_node.h"

#include "complex_map_nearest_sampler.h"

namespace manta {

    class ComplexMapNearestSamplerNode : public ObjectReferenceNode<ComplexMapSampler> {
    public:
        ComplexMapNearestSamplerNode() {
            /* void */
        }

        virtual ~ComplexMapNearestSamplerNode() {
            /* void */
        }

    protected:
        virtual void _evaluate() {
            setOutput(&m_sampler);
        }

    protected:
        ComplexMapNearestSampler m_sampler;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_NEAREST_SAMPLER_NODE_H */
