#include <fraunhofer_diffraction.h>

#include <image_byte_buffer.h>
#include <aperture.h>
#include <complex_map_2d.h>

manta::FraunhoferDiffraction::FraunhoferDiffraction() {
	m_physicalRadius = (math::real)1.0;
}

manta::FraunhoferDiffraction::~FraunhoferDiffraction() {

}

manta::math::real manta::FraunhoferDiffraction::getExtents(const math::Vector &reference) const {
	if (math::getX(reference) == 0 || math::getY(reference) == 0 || math::getZ(reference) == 0) return (math::real)0.0;

	return (math::real)0.5;
}

manta::math::Vector manta::FraunhoferDiffraction::sample(math::real x, math::real y) const {
	return m_diffractionPattern.sample(x / m_physicalRadius + 0.5, y / m_physicalRadius + 0.5);
}

void manta::FraunhoferDiffraction::generate(const Aperture *aperture, int precision) {
	int matrixWidth = precision * 2;

	m_diffractionPattern.initialize(matrixWidth, matrixWidth);
	m_apertureFunction.initialize(matrixWidth, matrixWidth);

	math::real apR = aperture->getRadius();
	math::real sampleR = apR * 3;

	math::real dr = sampleR / matrixWidth;

	for (int u = -precision; u < precision; u++) {
		for (int v = -precision; v < precision; v++) {
			math::real ru = (u + (math::real)0.5) * dr;
			math::real rv = (v + (math::real)0.5) * dr;

			int iu = u + precision;
			int iv = v + precision;

			bool apF = aperture->filter(ru, rv);
			if (apF) m_apertureFunction.set((math::real)1.0, iu, iv);
		}
	}

	ScalarMap2D empty;
	empty.initialize(matrixWidth, matrixWidth);

	ComplexMap2D apertureC;
	ComplexMap2D diffractionPattern;
	apertureC.copy(&m_apertureFunction);

	apertureC.fft(&diffractionPattern);

	m_diffractionPattern.initialize(matrixWidth, matrixWidth);

	for (int i = 0; i < matrixWidth; i++) {
		for (int j = 0; j < matrixWidth; j++) {
			math::real intensity = diffractionPattern.get(i, j).r * diffractionPattern.get(i, j).r;
			m_diffractionPattern.set(math::loadScalar(intensity), i, j);
		}
	}

	// Rearrange quadrants
	/*
	for (int u = 0; u < precision; u++) {
		for (int v = 0; v < precision * 2; v++) {
			int u1 = u;
			int v1 = v;
			int u2 = u + precision;
			int v2 = v;

			math::Vector tempIntensity = m_diffractionPattern.get(u1, v1);
			math::Vector intensity1 = m_diffractionPattern.get(u2, v2);
			m_diffractionPattern.set(intensity1, u1, v1);
			m_diffractionPattern.set(tempIntensity, u2, v2);
		}
	}

	for (int u = 0; u < precision * 2; u++) {
		for (int v = 0; v < precision; v++) {
			int u1 = u;
			int v1 = v;
			int u2 = u;
			int v2 = v + precision;

			math::Vector tempIntensity = m_diffractionPattern.get(u1, v1);
			math::Vector intensity1 = m_diffractionPattern.get(u2, v2);
			m_diffractionPattern.set(intensity1, u1, v1);
			m_diffractionPattern.set(tempIntensity, u2, v2);
		}
	}*/

	normalize();

	empty.destroy();
	apertureC.destroy();
	diffractionPattern.destroy();
}

void manta::FraunhoferDiffraction::destroy() {
	m_diffractionPattern.destroy();
	m_apertureFunction.destroy();
}

manta::math::Vector manta::FraunhoferDiffraction::getTotalFlux() const {
	int width = m_diffractionPattern.getWidth();
	int height = m_diffractionPattern.getHeight();

	math::real dw = m_physicalRadius / width;

	math::Vector total = math::constants::Zero;
	math::Vector ds = math::loadScalar(dw * dw);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			total = math::add(total, math::mul(ds, m_diffractionPattern.get(i, j)));
		}
	}

	return total;
}

void manta::FraunhoferDiffraction::normalize() {
	math::Vector totalFlux = getTotalFlux();
	math::Vector scale = math::div(math::constants::One, totalFlux);

	m_diffractionPattern.scale(scale);
}
