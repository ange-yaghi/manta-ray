#ifndef MANTARAY_RANDOM_SAMPLER_H
#define MANTARAY_RANDOM_SAMPLER_H

#include "sampler.h"

namespace manta {

    class RandomSampler : public Sampler {
    public:
        RandomSampler();
        virtual ~RandomSampler();

        virtual math::real generate1d();
        virtual math::Vector2 generate2d();

        virtual Sampler *clone() const;
    };

} /* namespace manta */

#endif /* MANTARAY_RANDOM_SAMPLER_H */
