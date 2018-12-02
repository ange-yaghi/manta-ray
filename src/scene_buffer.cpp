#include <scene_buffer.h>

#include <assert.h>

manta::SceneBuffer::SceneBuffer() {
	m_width = 0;
	m_height = 0;
	m_buffer = nullptr;
}

manta::SceneBuffer::~SceneBuffer() {
	assert(m_buffer == nullptr);
}

void manta::SceneBuffer::initialize(int width, int height) {
	assert(width != 0);
	assert(height != 0);

	m_width = width;
	m_height = height;
	m_buffer = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * width * height, 16);

	assert(m_buffer != nullptr);
}

void manta::SceneBuffer::destroy() {
	assert(m_buffer != nullptr);

	_aligned_free(m_buffer);

	// Reset member variables
	m_buffer = nullptr;
	m_width = 0;
	m_height = 0;
}

inline void manta::SceneBuffer::set(const math::Vector &v, int x, int y) {
	assert(x < m_width && x >= 0);
	assert(y < m_height && y >= 0);

	m_buffer[y * m_width + x] = v;
}

manta::math::Vector manta::SceneBuffer::sample(int x, int y) const {
	assert(x < m_width && x >= 0);
	assert(y < m_height && y >= 0);

	return m_buffer[y * m_width + x];
}

void manta::SceneBuffer::clone(SceneBuffer *target) const {
	target->initialize(m_width, m_height);
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			target->set(sample(x, y), x, y);
		}
	}
}

manta::math::real manta::SceneBuffer::getMax() const {
	math::real maxIntensity = (math::real)0.0;
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			math::real intensity = math::getScalar(math::magnitudeSquared3(sample(x, y)));
			if (intensity > maxIntensity) {
				maxIntensity = intensity;
			}
		}
	}

	return sqrt(maxIntensity);
}

manta::math::real manta::SceneBuffer::getMin() const {
	math::real minIntensity = math::constants::REAL_MAX;
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			math::real intensity = math::getScalar(math::magnitudeSquared3(sample(x, y)));
			if (intensity < minIntensity) {
				minIntensity = intensity;
			}
		}
	}

	return sqrt(minIntensity);
}

manta::math::Vector manta::SceneBuffer::getAverage() const {
	math::Vector average = math::constants::Zero;
	int n = 0;
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			average = math::add(average, math::div(math::sub(sample(x, y), average), math::loadScalar(R(n))));
		}
	}

	return average;
}

void manta::SceneBuffer::applyGammaCurve(math::real gamma) {
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			math::Vector fragment = sample(x, y);
			math::real r = pow(math::getX(fragment), gamma);
			math::real g = pow(math::getY(fragment), gamma);
			math::real b = pow(math::getZ(fragment), gamma);
			set(math::loadVector(r, g, b), x, y);
		}
	}
}

void manta::SceneBuffer::scale(math::real scale) {
	math::Vector scalev = math::loadScalar(scale);

	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			math::Vector fragment = sample(x, y);
			fragment = math::mul(fragment, scalev);
			set(fragment, x, y);
		}
	}
}
