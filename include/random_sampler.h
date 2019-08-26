#ifndef MANTARAY_RANDOM_SAMPLER_H
#define MANTARAY_RANDOM_SAMPLER_H

#include "sampler_2d.h"

namespace manta {

    class RandomSampler : public Sampler2d {
    public:
        RandomSampler();
        virtual ~RandomSampler();

        virtual void generateSamples(int sampleCount, math::Vector2 *target) const;
        virtual int getTotalSampleCount(int sampleCount) const;
    };

} /* namespace manta */

#endif /* MANTARAY_RANDOM_SAMPLER_H */
