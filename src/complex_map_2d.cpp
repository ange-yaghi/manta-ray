#include <complex_map_2d.h>

#include <standard_allocator.h>
#include <image_byte_buffer.h>
#include <signal_processing.h>
#include <scalar_map_2d.h>
#include <image_plane.h>
#include <vector_map_2d.h>

#include <assert.h>

manta::ComplexMap2D::ComplexMap2D() {
	m_data = nullptr;
	m_width = 0;
	m_height = 0;
}

manta::ComplexMap2D::~ComplexMap2D() {
	assert(m_data == nullptr);
}

void manta::ComplexMap2D::initialize(int width, int height, math::real value) {
	m_width = width;
	m_height = height;

	m_data = StandardAllocator::Global()->allocate<math::Complex>(m_width * m_height, 16);

	for (int i = 0; i < m_width * m_height; i++) {
		m_data[i] = { (math::real)0.0, (math::real)0.0 };
	}
}

void manta::ComplexMap2D::destroy() {
	StandardAllocator::Global()->aligned_free(m_data, m_width * m_height);

	m_data = nullptr;
	m_width = 0;
	m_height = 0;
}

manta::math::Complex manta::ComplexMap2D::get(int u, int v) const {
	assert(m_data != nullptr);

	return m_data[v * m_width + u];
}

void manta::ComplexMap2D::set(const math::Complex &value, int u, int v) {
	assert(m_data != nullptr);

	m_data[v * m_width + u] = value;
}

manta::math::Complex manta::ComplexMap2D::sampleDiscrete(math::real_d ku, math::real_d kv) const {
	assert(m_data != nullptr);

	// Very simple sampling for now
	int ik_x = (int)(ku + 0.5);
	int ik_y = (int)(kv + 0.5);

	return m_data[ik_y * m_width + ik_x];
}

void manta::ComplexMap2D::fillByteBuffer(ImageByteBuffer *target, Margins *margins) const {
	int startX, startY;
	int width, height;

	if (margins != nullptr) {
		width = margins->width;
		height = margins->height;
		startX = margins->left;
		startY = margins->top;
	}
	else {
		width = m_width;
		height = m_height;
		startX = 0;
		startY = 0;
	}

	target->initialize(width, height);

	for (int u = 0; u < width; u++) {
		for (int v = 0; v < height; v++) {
			ImageByteBuffer::Color color;
			math::Complex value = get(u + startX, v + startY);

			target->convertToColor(math::loadVector(value.r, (math::real)0.0, value.i), &color);
			target->setPixel(v, u, color);
		}
	}
}

void manta::ComplexMap2D::roll(ComplexMap2D *target) const {
	target->initialize(m_width, m_height);

	int offsetX = m_width / 2;
	int offsetY = m_height / 2;

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			math::Complex v = get(i, j);
			target->set(v, (i + offsetX) % m_width, (j + offsetY) % m_height);
		}
	}
}

void manta::ComplexMap2D::fft(ComplexMap2D *target) const {
	target->initialize(m_width, m_height);

	int width = m_width;
	int height = m_height;
	int minSpace = (width > height) ? width : height;

	math::Complex *inputBuffer = StandardAllocator::Global()->allocate<math::Complex>(minSpace);
	math::Complex *outputBuffer = StandardAllocator::Global()->allocate<math::Complex>(minSpace);

	/* Transform the rows */
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			math::Complex val = get(i, j);
			inputBuffer[i] = val;
		}

		//calculateFFT1D(realBuffer, imaginaryBuffer, width);
		NaiveFFT::fft(inputBuffer, outputBuffer, width);
		for (int i = 0; i < width; i++) {
			math::Complex result;
			result = outputBuffer[i];

			target->set(result, i, j);
		}
	}

	/* Transform the columns */
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			math::Complex val = target->get(i, j);
			inputBuffer[j] = val;
		}

		//calculateFFT1D(realBuffer, imaginaryBuffer, height);
		NaiveFFT::fft(inputBuffer, outputBuffer, height);
		for (int j = 0; j < height; j++) {
			math::Complex result;
			result = outputBuffer[j];

			target->set(result, i, j);
		}
	}

	StandardAllocator::Global()->free(inputBuffer, minSpace);
	StandardAllocator::Global()->free(outputBuffer, minSpace);
}

void manta::ComplexMap2D::inverseFft(ComplexMap2D *target) const {
	target->initialize(m_width, m_height);

	int width = m_width;
	int height = m_height;
	int minSpace = (width > height) ? width : height;

	math::Complex *inputBuffer = StandardAllocator::Global()->allocate<math::Complex>(minSpace);
	math::Complex *outputBuffer = StandardAllocator::Global()->allocate<math::Complex>(minSpace);

	// Transform the rows
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			math::Complex val = get(i, j);
			inputBuffer[i] = val;
		}

		NaiveFFT::fft_inverse(inputBuffer, outputBuffer, width);
		for (int i = 0; i < width; i++) {
			math::Complex result;
			result = outputBuffer[i];

			target->set(result, i, j);
		}
	}

	// Transform the columns
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			math::Complex val = target->get(i, j);
			inputBuffer[j] = val;
		}

		NaiveFFT::fft_inverse(inputBuffer, outputBuffer, height);
		for (int j = 0; j < height; j++) {
			math::Complex result;
			result = outputBuffer[j];

			target->set(result, i, j);
		}
	}

	StandardAllocator::Global()->free(inputBuffer, minSpace);
	StandardAllocator::Global()->free(outputBuffer, minSpace);
}

void manta::ComplexMap2D::cft(ComplexMap2D *target, math::real_d physicalWidth, math::real_d physicalHeight) const {
	target->initialize(m_width, m_height);

	int horizontalSamples = m_width;
	int verticalSamples = m_height;

	math::real_d w_inv = 1 / physicalWidth;
	math::real_d h_inv = 1 / physicalHeight;

	math::real_d fs_x = horizontalSamples * w_inv;
	math::real_d fs_y = verticalSamples * h_inv;

	math::real_d fs_inv_s = 1 / (fs_x * fs_y);

	int halfHorizontalSamples = horizontalSamples / 2;
	int halfVerticalSamples = verticalSamples / 2;

	math::real_d halfWidth = physicalWidth / 2;
	math::real_d halfHeight = physicalHeight / 2;

	for (int kx = -halfHorizontalSamples; kx < halfHorizontalSamples; kx++) {
		// frequency_x = kx / physicalWidth

		for (int ky = -halfVerticalSamples; ky < halfVerticalSamples; ky++) {
			// frequency_y = ky / physicalHeight

			// phase = exp(2 * pi * (frequency_x * (w / 2) + frequency_y * (h / 2)))
			math::Complex phaseTransformation;
			//math::real_d phase = math::constants::TWO_PI * (freq_x * halfWidth + freq_y * halfHeight);
			int phase = kx + ky;
			//phaseTransformation.r = ::cos(phase);
			//phaseTransformation.i = ::sin(phase);
			phaseTransformation.r = (phase % 2 == 0) ? (math::real_d)1.0 : (math::real_d)-1.0;
			phaseTransformation.i = (math::real_d)0.0;

			int mapIndexX = (kx + horizontalSamples) % horizontalSamples;
			int mapIndexY = (ky + verticalSamples) % verticalSamples;

			math::Complex dftApprox = get(mapIndexX, mapIndexY);
			dftApprox = dftApprox * phaseTransformation * math::Complex(fs_inv_s, 0.0);

			target->set(dftApprox, mapIndexX, mapIndexY);
		}
	}
}

void manta::ComplexMap2D::magnitude() {
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			math::Complex v = get(i, j);
			v = v * v.conjugate();
			set(v, i, j);
		}
	}
}

void manta::ComplexMap2D::multiply(const math::Complex &s) {
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			math::Complex v = get(i, j);
			set(v * s, i, j);
		}
	}
}

void manta::ComplexMap2D::resizeSafe(ComplexMap2D *target, Margins *margins) const {
	int minWidth = m_width * 2;
	int minHeight = m_height * 2;

	int width = 1, height = 1;
	while (width < minWidth) width *= 2;
	while (height < minHeight) height *= 2;

	int marginX = (width - m_width) / 2;
	int marginY = (height - m_height) / 2;

	margins->height = m_height;
	margins->width = m_width;
	margins->left = marginX;
	margins->top = marginY;

	target->initialize(width, height);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i >= marginX && (i - marginX) < m_width) {
				if (j >= marginY && (j - marginY) < m_height) {
					target->set(get(i - marginX, j - marginY), i, j);
				}
			}
		}
	}
}

void manta::ComplexMap2D::resize(int width, int height, ComplexMap2D *target, Margins *margins) const {
	int marginX = (width - m_width) / 2;
	int marginY = (height - m_height) / 2;

	target->initialize(width, height);

	if (margins != nullptr) {
		margins->height = height;
		margins->width = width;
		margins->left = marginX;
		margins->top = marginY;
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (i >= marginX && (i - marginX) < m_width) {
				if (j >= marginY && (j - marginY) < m_height) {
					target->set(get(i - marginX, j - marginY), i, j);
				}
			}
		}
	}
}

void manta::ComplexMap2D::multiply(const ComplexMap2D *b) {
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(get(i, j) * b->get(i, j), i, j);
		}
	}
}

void manta::ComplexMap2D::copy(const ComplexMap2D *b) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(b->get(i, j), i, j);
		}
	}
}

void manta::ComplexMap2D::copy(const ScalarMap2D *b) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(math::Complex(b->get(i, j), (math::real)0.0), i, j);
		}
	}
}

void manta::ComplexMap2D::copy(const ImagePlane *b, int channel) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(math::Complex(math::get(b->sample(i, j), channel), (math::real)0.0), i, j);
		}
	}
}

void manta::ComplexMap2D::copy(const VectorMap2D *b, int channel) {
	initialize(b->getWidth(), b->getHeight());

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(math::Complex(math::get(b->get(i, j), channel), (math::real)0.0), i, j);
		}
	}
}
