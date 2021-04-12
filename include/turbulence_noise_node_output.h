#ifndef MANTARAY_TURBULENCE_NOISE_NODE_OUTPUT_H
#define MANTARAY_TURBULENCE_NOISE_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "manta_math.h"

namespace manta {

    class TurbulenceNoiseNodeOutput : public VectorNodeOutput {
    public:
        TurbulenceNoiseNodeOutput();
        virtual ~TurbulenceNoiseNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        static math::Vector fractionalBrownianMotion(const math::Vector &input, math::real omega, int octaves);

        piranha::pNodeInput *getInputConnection() { return &m_input; }
        piranha::pNodeInput *getOmegaConnection() { return &m_omega; }
        piranha::pNodeInput *getOctavesConnection() { return &m_octaves; }

    protected:
        piranha::pNodeInput m_input;
        piranha::pNodeInput m_omega;
        piranha::pNodeInput m_octaves;
    };

} /* namespace manta */

#endif /* MANTARAY_TURBULENCE_NOISE_NODE_OUTPUT_H */
