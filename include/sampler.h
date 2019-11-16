#ifndef MANTARAY_SAMPLER_H
#define MANTARAY_SAMPLER_H

#include "manta_math.h"

namespace manta {

    class Sampler {
    public:
        Sampler();
        ~Sampler();

        virtual void startPixelSession();
        virtual void endSample();

        virtual math::real generate1d() = 0;
        virtual math::Vector2 generate2d() = 0;

    public:
        int m_currentPixelSample;
    };

} /* namespace manta */

#endif /* MANTARAY_SAMPLER_H */
