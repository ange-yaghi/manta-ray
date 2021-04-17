#ifndef MANTARAY_SAMPLER_H
#define MANTARAY_SAMPLER_H

#include "object_reference_node.h"

#include "manta_math.h"

#include <random>

namespace manta {

    class Sampler : public ObjectReferenceNode<Sampler> {
    public:
        Sampler();
        ~Sampler();

        virtual void startPixelSession();
        virtual bool startNextSample();

        virtual math::real generate1d() = 0;
        virtual math::Vector2 generate2d() = 0;

        math::real uniformRandom();
        uint32_t uniformRandomInt(uint32_t range);

        virtual Sampler *clone() const = 0;

        void setSamplesPerPixel(int samplesPerPixel) { m_samplesPerPixel = samplesPerPixel; }
        int getSamplesPerPixel() const { return m_samplesPerPixel; }

        void seed(unsigned int seed);

        int getCurrentPixelSample() const { return m_currentPixelSample; }

    protected:
        int m_currentPixelSample;

        piranha::pNodeInput m_samplesPerPixelInput;
        int m_samplesPerPixel;

        std::mt19937 m_rng;

    protected:
        virtual void _evaluate();
        virtual void registerInputs();
    };

} /* namespace manta */

#endif /* MANTARAY_SAMPLER_H */
