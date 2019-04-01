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
	if (iv < 0 || iv >= m_width) return math::constants::Zero;

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
