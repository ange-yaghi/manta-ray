#include <image_plane.h>

#include <convolution.h>

#include <assert.h>
#include <iostream>

manta::ImagePlane::ImagePlane() {
	m_width = 0;
	m_height = 0;
	m_buffer = nullptr;
}

manta::ImagePlane::~ImagePlane() {
	assert(m_buffer == nullptr);
}

void manta::ImagePlane::initialize(int width, int height, math::real physicalWidth, math::real physicalHeight) {
	assert(width != 0);
	assert(height != 0);

	m_width = width;
	m_height = height;

	m_physicalWidth = physicalWidth;
	m_physicalHeight = physicalHeight;

	m_buffer = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * width * height, 16);

	assert(m_buffer != nullptr);
}

void manta::ImagePlane::destroy() {
	assert(m_buffer != nullptr);

	_aligned_free(m_buffer);

	// Reset member variables
	m_buffer = nullptr;
	m_width = 0;
	m_height = 0;
}

bool manta::ImagePlane::checkPixel(int x, int y) const {
	return (x < m_width && x >= 0) && (y < m_height && y >= 0);
}

inline void manta::ImagePlane::set(const math::Vector &v, int x, int y) {
	assert(x < m_width && x >= 0);
	assert(y < m_height && y >= 0);

	m_buffer[y * m_width + x] = v;
}

manta::math::Vector manta::ImagePlane::sample(int x, int y) const {
	assert(x < m_width && x >= 0);
	assert(y < m_height && y >= 0);

	return m_buffer[y * m_width + x];
}

void manta::ImagePlane::clone(ImagePlane *target) const {
	target->initialize(m_width, m_height, m_physicalWidth, m_physicalHeight);
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			target->set(sample(x, y), x, y);
		}
	}
}

void manta::ImagePlane::cloneEmpty(ImagePlane *target) const {
	target->initialize(m_width, m_height, m_physicalWidth, m_physicalHeight);
}

void manta::ImagePlane::clear(const math::Vector &v) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			set(v, x, y);
		}
	}
}

manta::math::real manta::ImagePlane::getMax() const {
	math::real maxIntensity = (math::real)0.0;
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			math::real intensity = math::getScalar(math::magnitudeSquared3(sample(x, y)));
			if (intensity > maxIntensity) {
				maxIntensity = intensity;
			}
		}
	}

	return sqrt(maxIntensity);
}

manta::math::real manta::ImagePlane::getMin() const {
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

manta::math::Vector manta::ImagePlane::getAverage() const {
	math::Vector average = math::constants::Zero;
	int n = 0;
	for (int x = 0; x < (m_width); x++) {
		for (int y = 0; y < (m_height); y++) {
			average = math::add(average, math::div(math::sub(sample(x, y), average), math::loadScalar((math::real)n)));
		}
	}

	return average;
}

void manta::ImagePlane::applyGammaCurve(math::real gamma) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			math::Vector fragment = sample(x, y);
			math::real r = pow(math::getX(fragment), gamma);
			math::real g = pow(math::getY(fragment), gamma);
			math::real b = pow(math::getZ(fragment), gamma);
			set(math::loadVector(r, g, b), x, y);
		}
	}
}

void manta::ImagePlane::convolution(const Convolution *convolution, ImagePlane *target) {
	cloneEmpty(target);
	target->clear();

	math::real px = m_physicalWidth / m_width;
	math::real py = m_physicalHeight / m_height;
	math::real minR = (px < py) ? px : py;

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			math::Vector fragment = sample(x, y);

			math::real extents = convolution->getExtents(fragment);
			if (extents < minR) continue;

			int pex = (int)(extents / px + (math::real)0.5);
			int pey = (int)(extents / py + (math::real)0.5);

			for (int ex = -pex; ex <= pex; ex++) {
				for (int ey = -pey; ey <= pey; ey++) {
					if (ex == 0 && ey == 0) continue;

					int pixelX = ex + x;
					int pixelY = ey + y;

					if (pixelX < 0 || pixelX >= m_width) continue;
					if (pixelY < 0 || pixelY >= m_height) continue;

					math::real ds = px * py;
					//ds = 1.0;

					math::Vector convolutionMul = math::mul(convolution->sample(ex * px, ey * py), math::loadScalar(ds));
					math::Vector base = target->sample(pixelX, pixelY);

					target->set(math::add(base, math::mul(convolutionMul, fragment)), pixelX, pixelY);
				}
			}
		}
		
		int pixelsProcessed = x * m_height;
		std::cout << "Post process: " << pixelsProcessed << "/" << (m_width * m_height) << std::endl;
	}
}

void manta::ImagePlane::add(const ImagePlane *b) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			math::Vector fragment = sample(x, y);
			math::Vector fragmentB = b->sample(x, y);
			fragment = math::add(fragment, fragmentB);
			set(fragment, x, y);
		}
	}
}

void manta::ImagePlane::scale(math::real scale) {
	math::Vector scalev = math::loadScalar(scale);

	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			math::Vector fragment = sample(x, y);
			fragment = math::mul(fragment, scalev);
			set(fragment, x, y);
		}
	}
}
