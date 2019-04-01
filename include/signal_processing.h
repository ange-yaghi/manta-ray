#ifndef SIGNAL_PROCESSING_H
#define SIGNAL_PROCESSING_H

#include <manta_math.h>
#include <complex_math.h>

namespace manta {

	class NaiveFFT {
	public:
		static void fft(const math::Complex *input, math::Complex *target, int n);
		static void fft_inverse(const math::Complex *input, math::Complex *target, int n);

	private:
		static void fft_child(const math::Complex *input, math::Complex *target, int n, int s);
	};

} /* namespace manta */

#endif /* SIGNAL_PROCESSING_H */
