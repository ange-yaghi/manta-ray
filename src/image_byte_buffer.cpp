#include <image_byte_buffer.h>

#include <standard_allocator.h>
#include <scene_buffer.h>

#include <assert.h>
#include <cmath>

manta::ImageByteBuffer::ImageByteBuffer() {
	m_width = 0;
	m_height = 0;
	m_pitch = 0;
	m_buffer = nullptr;
}

manta::ImageByteBuffer::~ImageByteBuffer() {
	assert(m_buffer == nullptr);
}

void manta::ImageByteBuffer::initialize(SceneBuffer *sceneBuffer) {
	m_width = sceneBuffer->getWidth();
	m_height = sceneBuffer->getHeight();
	m_pitch = 4;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);
	const math::Vector *raw = sceneBuffer->getBuffer();

	for (int i = 0; i < sceneBuffer->getHeight(); i++) {
		for (int j = 0; j < sceneBuffer->getWidth(); j++) {
			Color c;
			math::Vector v;

			v = raw[i * sceneBuffer->getWidth() + j];
			convertToColor(v, &c);

			setPixel(i, j, c);
		}
	}
}

void manta::ImageByteBuffer::initialize(unsigned char *buffer, int width, int height, int pitch) {
	m_width = width;
	m_height = height;
	m_pitch = pitch;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);

	memcpy((void *)m_buffer, (void *)buffer, m_width * m_height * m_pitch);
}

void manta::ImageByteBuffer::free() {
	StandardAllocator::Global()->free(m_buffer, m_width * m_height * m_pitch);

	m_width = 0;
	m_height = 0;
	m_pitch = 0;
	m_buffer = nullptr;
}

void manta::ImageByteBuffer::setPixel(int row, int column, const Color &c) {
	int offset = (row * m_width + column) * m_pitch;

	m_buffer[offset] = c.r;
	m_buffer[offset + 1] = c.g;
	m_buffer[offset + 2] = c.b;
	m_buffer[offset + 3] = c.a;
}

void manta::ImageByteBuffer::convertToColor(const math::Vector &v, Color *c) const {
	math::Vector q = math::mul(v, math::loadScalar((math::real)255.0));

	int r = lround(math::getX(q));
	int g = lround(math::getY(q));
	int b = lround(math::getZ(q));
	int a = lround(math::getW(q));

	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	if (a > 255) a = 255;

	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	if (a < 0) a = 0;

	c->r = r;
	c->g = g;
	c->b = b;
	c->a = a;
}
