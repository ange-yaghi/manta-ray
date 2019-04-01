#include <scalar_map_2d.h>

#include <standard_allocator.h>
#include <image_byte_buffer.h>
#include <signal_processing.h>
#include <vector_map_2d.h>

#include <assert.h>

manta::ScalarMap2D::ScalarMap2D() {
	m_data = nullptr;
}

manta::ScalarMap2D::~ScalarMap2D() {
	assert(m_data == nullptr);
}

void manta::ScalarMap2D::initialize(int width, int height, math::real value) {
	m_width = width;
	m_height = height;

	m_data = StandardAllocator::Global()->allocate<math::real>(m_width * m_height, 16);

	for (int i = 0; i < m_width * m_height; i++) {
		m_data[i] = (math::real)0.0;
	}
}

void manta::ScalarMap2D::destroy() {
	StandardAllocator::Global()->aligned_free(m_data, m_width * m_height);

	m_data = nullptr;
	m_width = 0;
	m_height = 0;
}

manta::math::real manta::ScalarMap2D::sample(math::real u, math::real v) const {
	// Very simple sampling for now
	int iu = (int)(u + (math::real)0.5);
	int iv = (int)(v + (math::real)0.5);

	if (iu < 0 || iu >= m_width) return (math::real)0.0;
	if (iv < 0 || iv >= m_width) return (math::real)0.0;

	return get(iu, iv);
}

manta::math::real manta::ScalarMap2D::get(int u, int v) const {
	assert(m_data != nullptr);

	return m_data[v * m_width + u];
}

void manta::ScalarMap2D::set(math::real value, int u, int v) {
	assert(m_data != nullptr);

	m_data[v * m_width + u] = value;
}

void manta::ScalarMap2D::fillByteBuffer(ImageByteBuffer *target) const {
	target->initialize(m_data, m_width, m_height);
}
//
//void manta::ScalarMap2D::calculateFFT1D(math::real *real, math::real *imaginary, int width) {
//	long i1, i2, l1, l2;
//	double c1, c2, tx, ty, t1, t2, u1, u2, z;
//
//	/* Calculate the number of points */
//	int n = width;
//	int m = 0;
//	int temp = 1;
//	while (temp < width) {
//		m++;
//		temp <<= 1;
//	}
//
//	/* Do the bit reversal */
//	i2 = n >> 1;
//	int j = 0;
//	for (int i = 0; i < n - 1; i++) {
//		if (i < j) {
//			tx = real[i];
//			ty = imaginary[i];
//			real[i] = real[j];
//			imaginary[i] = imaginary[j];
//			real[j] = tx;
//			imaginary[j] = ty;
//		}
//		int k = i2;
//		while (k <= j) {
//			j -= k;
//			k >>= 1;
//		}
//		j += k;
//	}
//
//	/* Compute the FFT */
//	c1 = -1.0;
//	c2 = 0.0;
//	l2 = 1;
//	for (int l = 0; l < m; l++) {
//		l1 = l2;
//		l2 <<= 1;
//		u1 = 1.0;
//		u2 = 0.0;
//		for (int j = 0; j < l1; j++) {
//			for (int i = j; i < n; i += l2) {
//				i1 = i + l1;
//				t1 = u1 * real[i1] - u2 * imaginary[i1];
//				t2 = u1 * imaginary[i1] + u2 * real[i1];
//				real[i1] = real[i] - t1;
//				imaginary[i1] = imaginary[i] - t2;
//				real[i] += t1;
//				imaginary[i] += t2;
//			}
//			z = u1 * c1 - u2 * c2;
//			u2 = u1 * c2 + u2 * c1;
//			u1 = z;
//		}
//		c2 = -sqrt((1.0 - c1) / 2.0);
//		c1 = sqrt((1.0 + c1) / 2.0);
//	}
//
//	/* Scaling for forward transform */
//	for (int i = 0; i < n; i++) {
//		real[i] /= n;
//		imaginary[i] /= n;
//	}
//}
//
//void manta::ScalarMap2D::fft(ScalarMap2D *real, ScalarMap2D *imaginary, ScalarMap2D *targetReal, ScalarMap2D *targetImaginary) {
//	targetReal->initialize(real->getWidth(), real->getHeight());
//	targetImaginary->initialize(real->getWidth(), real->getHeight());
//
//	int m, twopm;
//	math::real *realBuffer, *imaginaryBuffer;
//
//	int width = real->getWidth();
//	int height = real->getHeight();
//	int minSpace = (width > height) ? width : height;
//
//	realBuffer = StandardAllocator::Global()->allocate<math::real>(width);
//	imaginaryBuffer = StandardAllocator::Global()->allocate<math::real>(width);
//
//	math::real *tempBufferReal = StandardAllocator::Global()->allocate<math::real>(minSpace);
//	math::real *tempBufferImaginary = StandardAllocator::Global()->allocate<math::real>(minSpace);
//
//	/* Transform the rows */
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			realBuffer[i] = real->get(i, j);
//			imaginaryBuffer[i] = imaginary->get(i, j);
//		}
//
//		//calculateFFT1D(realBuffer, imaginaryBuffer, width);
//		NaiveFFT::fft(realBuffer, imaginaryBuffer, tempBufferReal, tempBufferImaginary, width);
//		for (int i = 0; i < width; i++) {
//			targetReal->set(tempBufferReal[i], i, j);
//			targetImaginary->set(tempBufferImaginary[i], i, j);
//		}
//	}
//	StandardAllocator::Global()->free(realBuffer, width);
//	StandardAllocator::Global()->free(imaginaryBuffer, width);
//
//	/* Transform the columns */
//	realBuffer = StandardAllocator::Global()->allocate<math::real>(height);
//	imaginaryBuffer = StandardAllocator::Global()->allocate<math::real>(height);
//
//	for (int i = 0; i < width; i++) {
//		for (int j = 0; j < height; j++) {
//			realBuffer[j] = targetReal->get(i, j);
//			imaginaryBuffer[j] = targetImaginary->get(i, j);
//		}
//
//		//calculateFFT1D(realBuffer, imaginaryBuffer, height);
//		NaiveFFT::fft(realBuffer, imaginaryBuffer, tempBufferReal, tempBufferImaginary, height);
//		for (int j = 0; j < height; j++) {
//			targetReal->set(tempBufferReal[j], i, j);
//			targetImaginary->set(tempBufferImaginary[j], i, j);
//		}
//	}
//	StandardAllocator::Global()->free(realBuffer, height);
//	StandardAllocator::Global()->free(imaginaryBuffer, height);
//
//	StandardAllocator::Global()->free(tempBufferReal, height);
//	StandardAllocator::Global()->free(tempBufferImaginary, height);
//}
//
//void manta::ScalarMap2D::inverseFft(ScalarMap2D *real, ScalarMap2D *imaginary, ScalarMap2D *targetReal, ScalarMap2D *targetImaginary) {
//	targetReal->initialize(real->getWidth(), real->getHeight());
//	targetImaginary->initialize(real->getWidth(), real->getHeight());
//
//	int m, twopm;
//	math::real *realBuffer, *imaginaryBuffer;
//
//	int width = real->getWidth();
//	int height = real->getHeight();
//	int minSpace = (width > height) ? width : height;
//
//	realBuffer = StandardAllocator::Global()->allocate<math::real>(width);
//	imaginaryBuffer = StandardAllocator::Global()->allocate<math::real>(width);
//
//	math::real *tempBufferReal = StandardAllocator::Global()->allocate<math::real>(minSpace);
//	math::real *tempBufferImaginary = StandardAllocator::Global()->allocate<math::real>(minSpace);
//
//	/* Transform the rows */
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			realBuffer[i] = real->get(i, j);
//			imaginaryBuffer[i] = imaginary->get(i, j);
//		}
//
//		//calculateFFT1D(realBuffer, imaginaryBuffer, width);
//		NaiveFFT::fft_inverse(realBuffer, imaginaryBuffer, tempBufferReal, tempBufferImaginary, width);
//		for (int i = 0; i < width; i++) {
//			targetReal->set(tempBufferReal[i], i, j);
//			targetImaginary->set(tempBufferImaginary[i], i, j);
//		}
//	}
//	StandardAllocator::Global()->free(realBuffer, width);
//	StandardAllocator::Global()->free(imaginaryBuffer, width);
//
//	/* Transform the columns */
//	realBuffer = StandardAllocator::Global()->allocate<math::real>(height);
//	imaginaryBuffer = StandardAllocator::Global()->allocate<math::real>(height);
//
//	for (int i = 0; i < width; i++) {
//		for (int j = 0; j < height; j++) {
//			realBuffer[j] = targetReal->get(i, j);
//			imaginaryBuffer[j] = targetImaginary->get(i, j);
//		}
//
//		//calculateFFT1D(realBuffer, imaginaryBuffer, height);
//		NaiveFFT::fft_inverse(realBuffer, imaginaryBuffer, tempBufferReal, tempBufferImaginary, height);
//		for (int j = 0; j < height; j++) {
//			targetReal->set(tempBufferReal[j], i, j);
//			targetImaginary->set(tempBufferImaginary[j], i, j);
//		}
//	}
//	StandardAllocator::Global()->free(realBuffer, height);
//	StandardAllocator::Global()->free(imaginaryBuffer, height);
//
//	StandardAllocator::Global()->free(tempBufferReal, height);
//	StandardAllocator::Global()->free(tempBufferImaginary, height);
//}

void manta::ScalarMap2D::multiply(const ScalarMap2D *b) {
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(get(i, j) * b->get(i, j), i, j);
		}
	}
}

void manta::ScalarMap2D::copy(const ScalarMap2D *b) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(b->get(i, j), i, j);
		}
	}
}

void manta::ScalarMap2D::copy(const VectorMap2D *b) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(math::getScalar(b->get(i, j)), i, j);
		}
	}
}
