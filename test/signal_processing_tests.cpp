#include <pch.h>

#include <manta_math.h>
#include <standard_allocator.h>
#include <signal_processing.h>
#include <complex_map_2d.h>
#include <fraunhofer_diffraction.h>

#include <utilities.h>

#include <random>

using namespace manta;

TEST(SignalProcessingTests, FourierTransformSanityCheck) {
	constexpr int SIZE = 1024;

	math::Complex *data = StandardAllocator::Global()->allocate<math::Complex>(SIZE);
	math::Complex *f_data = StandardAllocator::Global()->allocate<math::Complex>(SIZE);
	math::Complex *f_second_order = StandardAllocator::Global()->allocate<math::Complex>(SIZE);

	for (int i = 0; i < SIZE; i++) {
		data[i].r = rand() % 100;
		data[i].i = 0.f;
	}

	NaiveFFT::fft(data, f_data, SIZE);
	NaiveFFT::fft_inverse(f_data, f_second_order, SIZE);

	for (int i = 0; i < SIZE; i++) {
		EXPECT_NEAR(f_second_order[i].r, data[i].r, 1E-4);
		EXPECT_NEAR(f_second_order[i].i, 0.0f, 1E-4);
	}

	StandardAllocator::Global()->free(data, SIZE);
	StandardAllocator::Global()->free(f_data, SIZE);
	StandardAllocator::Global()->free(f_second_order, SIZE);
}

TEST(SignalProcessingTests, FourierTransform2D) {
	constexpr int SIZE = 1024;

	ComplexMap2D input, output, parity;
	input.initialize(SIZE, SIZE);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			input.set(math::Complex(rand() % 100, 0.f), i, j);
		}
	}

	input.fft(&output);
	output.inverseFft(&parity);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			math::Complex ref = input.get(i, j);
			math::Complex v = parity.get(i, j);

			EXPECT_NEAR(v.r, ref.r, 1E-4);
			EXPECT_NEAR(v.i, ref.i, 1E-4);
		}
	}

	input.destroy();
	output.destroy();
	parity.destroy();
}

TEST(SignalProcessingTests, ConvolutionTestNoOp) {
	constexpr int SIZE = 1024;

	ComplexMap2D input, convolution, pulse;
	input.initialize(SIZE, SIZE);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			input.set(math::Complex(rand() % 100 / 100.f, 0.f), i, j);
		}
	}

	pulse.initialize(SIZE, SIZE);
	pulse.set(math::Complex(1.f, 0.f), 0, 0);

	ComplexMap2D inputF, pulseF;

	input.fft(&inputF);
	pulse.fft(&pulseF);

	inputF.multiply(&pulseF);

	inputF.inverseFft(&convolution);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			math::Complex ref = input.get(i, j);
			math::Complex v = convolution.get(i, j);

			EXPECT_NEAR(v.r, ref.r, 1E-4);
			EXPECT_NEAR(v.i, ref.i, 1E-4);
		}
	}

	writeToJpeg(&input, std::string(TMP_PATH) + "input.jpg");
	writeToJpeg(&convolution, std::string(TMP_PATH) + "convolution.jpg");

	input.destroy();
	convolution.destroy();
	pulse.destroy();
	inputF.destroy();
	pulseF.destroy();
}

TEST(SignalProcessingTests, ConvolutionTestMul) {
	constexpr int SIZE = 1024;

	ComplexMap2D input, convolution, pulse;
	input.initialize(SIZE, SIZE);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			input.set(math::Complex(rand() % 100 / 200.f, 0.f), i, j);
		}
	}

	pulse.initialize(SIZE, SIZE);
	pulse.set(math::Complex(2.f, 0.f), 0, 0);

	ComplexMap2D inputF, pulseF;

	input.fft(&inputF);
	pulse.fft(&pulseF);

	inputF.multiply(&pulseF);

	inputF.inverseFft(&convolution);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			math::Complex ref = input.get(i, j);
			math::Complex v = convolution.get(i, j);

			EXPECT_NEAR(v.r, ref.r * 2, 1E-4);
			EXPECT_NEAR(v.i, ref.i * 2, 1E-4);
		}
	}

	input.destroy();
	convolution.destroy();
	pulse.destroy();
	inputF.destroy();
	pulseF.destroy();
}
