#include <pch.h>

#include <manta_math.h>
#include <standard_allocator.h>
#include <signal_processing.h>
#include <complex_map_2d.h>
#include <fraunhofer_diffraction.h>
#include <cft_estimator_2d.h>
#include <circular_aperture.h>
#include <polygonal_aperture.h>

#include <utilities.h>

#include <random>

#define ENABLE_OUTPUT (true)

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

TEST(SignalProcessingTests, FourierTransform2DMultithreaded) {
	constexpr int SIZE = 2048;

	ComplexMap2D input, output, parity;
	input.initialize(SIZE, SIZE);

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			input.set(math::Complex(rand() % 100, 0.f), i, j);
		}
	}

	input.fft_multithreaded(&output, 12);
	output.fft_multithreaded(&parity, 12, true);

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

TEST(SignalProcessingTests, CftApproximationBasicTest) {
	constexpr int SIZE = 1024;

	constexpr math::real boxWidth = 1.0f, boxHeight = 1.0f;
	constexpr math::real sampleWindow = 50.0f;
	
	ComplexMap2D apertureFunction;
	PolygonalAperture aperture;
	aperture.setRadius(::sqrt((boxWidth / 2) * (boxWidth / 2) + (boxHeight / 2) * (boxHeight / 2)));
	aperture.initialize(4, math::constants::PI / 4);

	math::real_d fs_x = SIZE / sampleWindow;
	math::real_d fs_y = SIZE / sampleWindow;

	// Sample the aperture function
	apertureFunction.initialize(SIZE, SIZE);

	math::real dx = sampleWindow / SIZE;
	math::real dy = sampleWindow / SIZE;
	math::real cx = sampleWindow / 2;
	math::real cy = sampleWindow / 2;

	for (int i = 0; i < SIZE; i++) {
		math::real x = i * dx - cx;
		for (int j = 0; j < SIZE; j++) {
			math::real y = j * dy - cy;

			if (aperture.filter(x, y)) {
				apertureFunction.set(math::Complex(1.0f, 0.0f), i, j);
			}
			else {
				apertureFunction.set(math::Complex(0.0f, 0.0f), i, j);
			}
		}
	}

	CftEstimator2D estimator;
	estimator.initialize(&apertureFunction, sampleWindow, sampleWindow);

	math::real_d averageDiff = 0.0f;
	math::real_d maxDiff = 0.0;

	ComplexMap2D exactFourierMap;
	exactFourierMap.initialize(SIZE, SIZE);
	for (int kx = -SIZE / 2; kx < SIZE / 2; kx++) {
		math::real_d freq_x = ((math::real_d)kx / SIZE) * fs_x;

		math::real_d sinc1;
		math::real_d sinc2;

		if (freq_x == 0.0) sinc1 = 1.0f;
		else sinc1 = ::sin(math::constants::PI * boxWidth * freq_x) / (math::constants::PI * boxWidth * freq_x);

		for (int ky = -SIZE / 2; ky < SIZE / 2; ky++) {
			math::real_d freq_y = ((math::real_d)ky / SIZE) * fs_y;

			if (freq_y == 0.0) sinc2 = 1.0f;
			else sinc2 = ::sin(math::constants::PI * boxHeight * freq_y) / (math::constants::PI * boxHeight * freq_y);

			math::real_d exactFourier = boxWidth * boxHeight * sinc1 * sinc2;
			math::real_d approxFourier = estimator.sample(freq_x, freq_y, fs_x / SIZE).r;

			math::real_d diff = ::abs(exactFourier - approxFourier);
			if (diff > maxDiff) maxDiff = diff;
			averageDiff += diff;

			exactFourierMap.set(math::Complex(exactFourier, 0.0f), (kx + SIZE) % SIZE, (ky + SIZE) % SIZE);
		}
	}

	averageDiff /= (SIZE * SIZE);

#if ENABLE_OUTPUT
	ComplexMap2D viewable;
	estimator.getApproximation()->roll(&viewable);
	viewable.magnitude();
	viewable.multiply(math::Complex(100.0f, 0.0f));
	writeToJpeg(&viewable, std::string(TMP_PATH) + "cft_approx.jpg");
	viewable.destroy();

	exactFourierMap.roll(&viewable);
	viewable.magnitude();
	viewable.multiply(math::Complex(100.0f, 0.0f));
	writeToJpeg(&viewable, std::string(TMP_PATH) + "cft_exact.jpg");
	viewable.destroy();
#endif /* ENABLE_OUTPUT */

	EXPECT_LT(averageDiff, 5E-3);
	EXPECT_LT(maxDiff, 1E-1);

	exactFourierMap.destroy();
	estimator.destroy();
	apertureFunction.destroy();
	aperture.destroy();
}

TEST(SignalProcessingTests, CftConvolutionBasicTest) {
	constexpr int SAMPLES = 1024;

	ComplexMap2D function;
	function.initialize(SAMPLES, SAMPLES);

	constexpr math::real ds = (10 / (math::real)SAMPLES);

	math::real total = 0.0;
	for (int i = -SAMPLES / 2; i < SAMPLES / 2; i++) {
		for (int j = -SAMPLES / 2; j < SAMPLES / 2; j++) {
			math::real r2 = ((i * i + j * j) / (math::real)(SAMPLES * SAMPLES)) * 10 * 10;

			int ki = i, kj = j;
			ki += SAMPLES / 2;
			kj += SAMPLES / 2;

			constexpr math::real radius = 0.0f;

			if (i == 0 && j == 0 && radius == 0.0) {
				function.set(math::Complex(1.0 , 0.0), ki, kj);
				total += 1.0;
			}
			else if (r2 < radius * radius) {
				function.set(math::Complex(1.0, 0.0), ki, kj);
				total += 1.0;
			}
			else {
				function.set(math::Complex(0.0, 0.0), ki, kj);
			}
		}
	}
	math::real totalEnergy = 1 / (ds * ds);
	function.multiply(math::Complex(totalEnergy / total, 0.0f));

	ComplexMap2D functionDft, functionCft;
	function.fft(&functionDft);
	functionDft.cft(&functionCft, 10, 10);

	ComplexMap2D conv, convDft, convCft;
	conv.initialize(SAMPLES, SAMPLES);
	for (int i = -SAMPLES / 2; i < SAMPLES / 2; i++) {
		for (int j = -SAMPLES / 2; j < SAMPLES / 2; j++) {
			int ki = i, kj = j;
			ki += SAMPLES / 2;
			kj += SAMPLES / 2;

			if (std::abs(i) * (math::real)10 / SAMPLES < 0.5 &&
				std::abs(j) * (math::real)10 / SAMPLES < 0.5) {
				conv.set(math::Complex(1.0, 0.0), ki, kj);
			}
			else {
				conv.set(math::Complex(0.0, 0.0), ki, kj);
			}
		}
	}

	conv.fft(&convDft);
	convDft.cft(&convCft, 10, 10);

	ComplexMap2D convFreq, convFinal;
	functionCft.multiply(&convCft);

	functionCft.inverseCft(&convFreq, 10, 10);
	convFreq.inverseFft(&convFinal);

#if ENABLE_OUTPUT
	writeToJpeg(&convFinal, std::string(TMP_PATH) + "cft_convolution/conv_final.jpg");
	writeToJpeg(&function, std::string(TMP_PATH) + "cft_convolution/function.jpg");
	writeToJpeg(&conv, std::string(TMP_PATH) + "cft_convolution/base.jpg");
#endif /* ENABLE_OUTPUT */

	for (int i = 0; i < SAMPLES; i++) {
		for (int j = 0; j < SAMPLES; j++) {
			math::Complex c = convFinal.get(i, j);
			math::Complex ref = conv.get(i, j);

			EXPECT_NEAR(c.r, ref.r, 1E-5);
			EXPECT_NEAR(c.i, ref.i, 1E-5);
		}
	}

	function.destroy();
	functionDft.destroy();
	functionCft.destroy();
	conv.destroy();
	convDft.destroy();
	convCft.destroy();
	convFreq.destroy();
	convFinal.destroy();
}

TEST(SignalProcessingTests, CftConvolutionSpecificTest) {
	constexpr int SAMPLES = 1024;

	ComplexMap2D function;
	function.initialize(SAMPLES, SAMPLES);

	constexpr math::real ds = (10 / (math::real)SAMPLES);

	math::real total = 0.0;
	for (int i = -SAMPLES / 2; i < SAMPLES / 2; i++) {
		for (int j = -SAMPLES / 2; j < SAMPLES / 2; j++) {
			math::real r2 = ((i * i + j * j) / (math::real)(SAMPLES * SAMPLES)) * 10 * 10;

			int ki = i, kj = j;
			ki += SAMPLES / 2;
			kj += SAMPLES / 2;

			constexpr math::real radius = 0.0f;

			if (i == 0 && j == 0 && radius == 0.0) {
				function.set(math::Complex(1.0, 0.0), ki, kj);
				total += 1.0;
			}
			else if (r2 < radius * radius) {
				function.set(math::Complex(1.0, 0.0), ki, kj);
				total += 1.0;
			}
			else {
				function.set(math::Complex(0.0, 0.0), ki, kj);
			}
		}
	}
	math::real totalEnergy = 1 / (ds * ds);
	function.multiply(math::Complex(totalEnergy / total, 0.0f));

	ComplexMap2D functionDft;
	function.fft(&functionDft);

	ComplexMap2D conv, convDft;
	conv.initialize(SAMPLES, SAMPLES);
	for (int i = -SAMPLES / 2; i < SAMPLES / 2; i++) {
		for (int j = -SAMPLES / 2; j < SAMPLES / 2; j++) {
			int ki = i, kj = j;
			ki += SAMPLES / 2;
			kj += SAMPLES / 2;

			if (std::abs(i) * (math::real)10 / SAMPLES < 0.5 &&
				std::abs(j) * (math::real)10 / SAMPLES < 0.5) {
				conv.set(math::Complex(1.0, 0.0), ki, kj);
			}
			else {
				conv.set(math::Complex(0.0, 0.0), ki, kj);
			}
		}
	}

	conv.fft(&convDft);

	ComplexMap2D convFinal;
	convDft.cftConvolve(&functionDft, 10.0f, 10.0f);
	convDft.inverseFft(&convFinal);

#if ENABLE_OUTPUT
	writeToJpeg(&convFinal, std::string(TMP_PATH) + "cft_convolution/conv_final.jpg");
	writeToJpeg(&function, std::string(TMP_PATH) + "cft_convolution/function.jpg");
	writeToJpeg(&conv, std::string(TMP_PATH) + "cft_convolution/base.jpg");
#endif /* ENABLE_OUTPUT */

	for (int i = 0; i < SAMPLES; i++) {
		for (int j = 0; j < SAMPLES; j++) {
			math::Complex c = convFinal.get(i, j);
			math::Complex ref = conv.get(i, j);

			EXPECT_NEAR(c.r, ref.r, 1E-5);
			EXPECT_NEAR(c.i, ref.i, 1E-5);
		}
	}

	function.destroy();
	functionDft.destroy();
	conv.destroy();
	convDft.destroy();
	convFinal.destroy();
}
