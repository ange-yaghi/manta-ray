#ifndef MANTARAY_COMPLEX_MAP_SAMPLE_NODE_H
#define MANTARAY_COMPLEX_MAP_SAMPLE_NODE_H

#include <piranha.h>

#include "complex_map_sample_node_output.h"
#include "complex_map_sampler.h"
#include "object_reference_node.h"

namespace manta {

    class ComplexMapSampleNode : public piranha::Node {
    public:
        ComplexMapSampleNode() {
            m_map = nullptr;
            m_sampler = nullptr;
        }

        virtual ~ComplexMapSampleNode() {
            /* void */
        }

    protected:
        virtual void _initialize() {
            /* void */
        }

        virtual void _evaluate() {
            ComplexMapSampler *sampler = getObject<ComplexMapSampler>(m_sampler);
            m_output.setSampler(sampler);
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutput(&m_output, "__out");
        }

        virtual void registerInputs() {
            registerInput(&m_map, "map");
            registerInput(m_output.getMapConnection(), "map");
            registerInput(&m_sampler, "sampler");
        }

    protected:
        piranha::pNodeInput m_map;
        piranha::pNodeInput m_sampler;

        ComplexMapSampleNodeOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_SAMPLE_NODE_H */
