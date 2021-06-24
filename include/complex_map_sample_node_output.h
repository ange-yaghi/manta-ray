#ifndef MANTARAY_COMPLEX_MAP_SAMPLE_NODE_OUTPUT_H
#define MANTARAY_COMPLEX_MAP_SAMPLE_NODE_OUTPUT_H

#include "complex_node_output.h"

#include "complex_map_2d.h"

namespace manta {

    class ComplexMapSampler;

    class ComplexMapSampleNodeOutput : public ComplexNodeOutput {
    public:
        ComplexMapSampleNodeOutput();
        virtual ~ComplexMapSampleNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        void registerInputs();

        virtual void _evaluateDimensions();

        void setSampler(ComplexMapSampler *sampler) { m_sampler = sampler; }
        ComplexMapSampler *getSampler() const { return m_sampler; }

        piranha::pNodeInput *getMapConnection() { return &m_map; }

    protected:
        piranha::pNodeInput m_map;
        ComplexMapSampler *m_sampler;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_SAMPLE_NODE_OUTPUT_H */
