#ifndef MANTARAY_PIXEL_BASED_SAMPLER_H
#define MANTARAY_PIXEL_BASED_SAMPLER_H

#include "sampler.h"

#include "manta_math.h"

#include <vector>

namespace manta {

    class PixelBasedSampler : public Sampler {
    public:
        PixelBasedSampler();
        ~PixelBasedSampler();

        void configure(int samplesPerPixel, int dimensionCount);

        virtual math::real generate1d();
        virtual math::Vector2 generate2d();

        virtual bool startNextSample();

    protected:
        std::vector<std::vector<math::real>> m_1dSamples;
        std::vector<std::vector<math::Vector2>> m_2dSamples;

        int m_current1dDimension;
        int m_current2dDimension;
    };

} /* namespace manta */

#endif /* MANTARAY_PIXEL_BASED_SAMPLER_H */
