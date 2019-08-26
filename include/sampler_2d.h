#ifndef MANTARAY_SAMPLER_2D_H
#define MANTARAY_SAMPLER_2D_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta {

    class StackAllocator;

    class Sampler2d : public ObjectReferenceNode<Sampler2d> {
    public:
        Sampler2d();
        ~Sampler2d();

        virtual void generateSamples(int sampleCount, math::Vector2 *target) const = 0;
        virtual int getTotalSampleCount(int sampleCount) const = 0;
    };

} /* namespace manta */

#endif /* MANTARAY_SAMPLER_2D_H */
