#include <signal_processing.h>

#include <standard_allocator.h>

void manta::NaiveFFT::fft(const math::Complex *input, math::Complex *target, int n) {
	fft_child(input, target, n, 1);
}

void manta::NaiveFFT::fft_inverse(const math::Complex *input, math::Complex *target, int n) {
	fft_child(input, target, n, 1);

	math::real inv_n = 1 / (math::real)n;

	// Apply scale and reverse direction
	for (int i = 0; i < n; i++) {
		target[i] = target[i] * inv_n;
	}

	for (int i = 1; i < n / 2; i++) {
		math::Complex temp = target[i];
		target[i] = target[n - i];
		target[i] = target[n - i];

		target[n - i] = temp;
		target[n - i] = temp;
	}
}

void manta::NaiveFFT::fft_child(const math::Complex *input, math::Complex *target, int n, int s) {
	if (n == 1) {
		target[0] = input[0];
		return;
	}
	else if (n / 2 == 1) {
		target[0] = input[0];
		target[n / 2] = input[s];
	}
	else {
		fft_child(input, target, n / 2, 2 * s);
		fft_child(input + s, target + n / 2, n / 2, 2 * s);
	}

	for (int k = 0; k < n / 2; k++) {
		math::Complex t = target[k];

		math::real angle = -math::constants::TWO_PI * k / (math::real)n;
		math::real cos_angle = ::cos(angle);
		math::real sin_angle = ::sin(angle);

		math::Complex x = target[k + n / 2];

		const math::real_d s1 = cos_angle * x.r - sin_angle * x.i;
		const math::real_d s2 = cos_angle * x.i + sin_angle * x.r;

		target[k].r = t.r + s1;
		target[k].i = t.i + s2;

		target[k + n / 2].r = t.r - s1;
		target[k + n / 2].i = t.i - s2;
	}
}
