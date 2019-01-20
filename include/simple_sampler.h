#ifndef SIMPLE_SAMPLER_H
#define SIMPLE_SAMPLER_H

#include <sampler_2d.h>

namespace manta {

	class SimpleSampler : public Sampler2D {
	public:
		SimpleSampler();
		~SimpleSampler();

		virtual void generateSamples(int sampleCount, math::Vector *target) const;
		virtual int getTotalSampleCount(int sampleCount) const;
	};

} /* namespace manta */

#endif /* SIMPLE_SAMPLER_H */
