#ifndef MANTARAY_PERLIN_NOISE_NODE_OUTPUT_H
#define MANTARAY_PERLIN_NOISE_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "manta_math.h"

namespace manta {

    class PerlinNoiseNodeOutput : public VectorNodeOutput {
    public:
        static const int NoisePermutationMapSize = 256;
        static const int NoisePermutation[];

    public:
        PerlinNoiseNodeOutput();
        virtual ~PerlinNoiseNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2D(int x, int y, void *target) const;

        // TODO: replace with common lerp across mantaray
        static math::real lerp(math::real s, math::real s0, math::real s1);
        static math::real smoothStep(math::real min, math::real max, math::real value);
        static math::real clamp(math::real s, math::real min, math::real max);

        static math::real noiseWeight(math::real t);
        static math::Vector noise(const math::Vector &coordinates);
        static math::real grad(int x, int y, int z, math::real dx, math::real dy, math::real dz);

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };

} /* namespace manta */

#endif /* MANTARAY_PERLIN_NOISE_NODE_OUTPUT_H */
