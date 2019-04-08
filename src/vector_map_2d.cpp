#include <vector_map_2d.h>

#include <standard_allocator.h>
#include <image_byte_buffer.h>

#include <assert.h>

manta::VectorMap2D::VectorMap2D() {
	m_data = nullptr;
}

manta::VectorMap2D::~VectorMap2D() {
	assert(m_data == nullptr);
}

void manta::VectorMap2D::initialize(int width, int height, const math::Vector &value) {
	m_width = width;
	m_height = height;

	m_data = StandardAllocator::Global()->allocate<math::Vector>(m_width * m_height, 16);

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			set(value, i, j);
		}
	}
}

void manta::VectorMap2D::destroy() {
	StandardAllocator::Global()->aligned_free(m_data, m_width * m_height);

	m_data = nullptr;
	m_width = 0;
	m_height = 0;
}

manta::math::Vector manta::VectorMap2D::sample(math::real u, math::real v) const {
	// Very simple sampling for now
	int iu = (int)(u * m_width + (math::real)0.5);
	int iv = (int)(v * m_height + (math::real)0.5);

	if (iu < 0 || iu >= m_width) return math::constants::Zero;
	if (iv < 0 || iv >= m_height) return math::constants::Zero;

	return get(iu, iv);
}

manta::math::Vector manta::VectorMap2D::get(int u, int v) const {
	assert(m_data != nullptr);

	return m_data[v * m_width + u];
}

void manta::VectorMap2D::set(const math::Vector &value, int u, int v) {
	assert(m_data != nullptr);

	m_data[v * m_width + u] = value;
}

void manta::VectorMap2D::fillByteBuffer(ImageByteBuffer *target) const {
	target->initialize(m_data, m_width, m_height);
}

void manta::VectorMap2D::scale(const math::Vector &s) {
	int pCount = m_width * m_height;
	for (int i = 0; i < pCount; i++) {
		m_data[i] = math::mul(s, m_data[i]);
	}
}

void manta::VectorMap2D::applyGamma(math::real gamma) {
	int pCount = m_width * m_height;
	for (int i = 0; i < pCount; i++) {
		math::Vector fragment = m_data[i];
		math::real r = pow(math::getX(fragment), gamma);
		math::real g = pow(math::getY(fragment), gamma);
		math::real b = pow(math::getZ(fragment), gamma);
		m_data[i] = math::loadVector(r, g, b);
	}
}

manta::math::real manta::VectorMap2D::getMaxMagnitude() const {
	math::real maxMagnitude = 0.0;
	int pCount = m_width * m_height;
	for (int i = 0; i < pCount; i++) {
		math::real mag = math::getScalar(math::magnitude(m_data[i]));
		
		if (mag > maxMagnitude) maxMagnitude = mag;
	}

	return maxMagnitude;
}

void manta::VectorMap2D::roll(VectorMap2D *target) const {
	target->initialize(m_width, m_height);

	int offsetX = m_width / 2;
	int offsetY = m_height / 2;

	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_height; j++) {
			math::Vector v = get(i, j);
			target->set(v, (i + offsetX) % m_width, (j + offsetY) % m_height);
		}
	}
}

void manta::VectorMap2D::copy(const VectorMap2D *source) {
	initialize(source->getWidth(), source->getHeight());
	int pCount = m_width * m_height;
	for (int i = 0; i < pCount; i++) {
		m_data[i] = source->m_data[i];
	}
}
