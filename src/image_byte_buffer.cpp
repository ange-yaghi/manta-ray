#include <image_byte_buffer.h>

#include <standard_allocator.h>
#include <image_plane.h>
#include <rgb_space.h>

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

void manta::ImageByteBuffer::initialize(const ImagePlane *sceneBuffer, bool correctGamma) {
	const math::Vector *raw = sceneBuffer->getBuffer();
	initialize(raw, sceneBuffer->getWidth(), sceneBuffer->getHeight(), correctGamma);
}

void manta::ImageByteBuffer::initialize(const unsigned char *buffer, int width, int height, int pitch) {
	m_width = width;
	m_height = height;
	m_pitch = pitch;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);

	memcpy((void *)m_buffer, (void *)buffer, m_width * m_height * m_pitch);
}

void manta::ImageByteBuffer::initialize(const math::Vector *buffer, int width, int height, bool correctGamma) {
	m_width = width;
	m_height = height;
	m_pitch = 4;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Color c;
			math::Vector v = buffer[i * width + j];
			convertToColor(v, &c, correctGamma);

			setPixel(i, j, c);
		}
	}
}

void manta::ImageByteBuffer::initialize(const math::real *buffer, int width, int height, bool correctGamma) {
	m_width = width;
	m_height = height;
	m_pitch = 4;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Color c;
			math::real v = buffer[i * width + j];
			convertToColor(math::loadScalar(v), &c, correctGamma);

			setPixel(i, j, c);
		}
	}
}

void manta::ImageByteBuffer::initialize(int width, int height) {
	m_width = width;
	m_height = height;
	m_pitch = 4;

	m_buffer = StandardAllocator::Global()->allocate<unsigned char>(m_width * m_height * m_pitch);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Color c;
			convertToColor(math::constants::Zero, &c, false);
			// Gamma is not corrected to save         ^^^^^
			// processing time in this trivial case

			setPixel(i, j, c);
		}
	}
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

void manta::ImageByteBuffer::convertToColor(const math::Vector &v, Color *c, bool correctGamma) const {
	math::real vr, vg, vb, va;
	vr = math::getX(v);
	vg = math::getY(v);
	vb = math::getZ(v);
	va = math::getW(v);

	if (correctGamma) {
		// Default to SRGB
		// TODO: make gamma correction generic
		vr = RgbSpace::srgb.applyGammaSrgb(vr);
		vg = RgbSpace::srgb.applyGammaSrgb(vg);
		vb = RgbSpace::srgb.applyGammaSrgb(vb);
		va = RgbSpace::srgb.applyGammaSrgb(va);
	}

	int r = lround(vr * 255);
	int g = lround(vg * 255);
	int b = lround(vb * 255);
	int a = lround(va * 255);

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
