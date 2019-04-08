#include <fraunhofer_diffraction.h>

#include <image_byte_buffer.h>
#include <aperture.h>
#include <complex_map_2d.h>
#include <cft_estimator_2d.h>

// Temp
#include <texture_node.h>
#include <intersection_point.h>

manta::FraunhoferDiffraction::FraunhoferDiffraction() {
	m_physicalSensorWidth = (math::real)1.0;
}

manta::FraunhoferDiffraction::~FraunhoferDiffraction() {

}

void manta::FraunhoferDiffraction::generate(const Aperture *aperture, int outputResolution, math::real physicalSensorWidth, const Settings *settingsIn) {
	Settings settings;
	if (settingsIn == nullptr) defaultSettings(&settings);
	else settings = *settingsIn;
	
	int MAX_SAMPLES = settings.maxSamples;
	int minWavelength = settings.minWaveLength;
	int maxWavelength = settings.maxWaveLength;
	int wavelengthStep = settings.wavelengthStep;

	math::real apertureRadius = aperture->getRadius();
	math::real apertureRadius_inv = 1 / apertureRadius;
	math::real sensorWidth = physicalSensorWidth;
	math::real sensorElementWidth = physicalSensorWidth / outputResolution;

	m_physicalSensorWidth = physicalSensorWidth;
	m_sensorElementWidth = sensorElementWidth;

	math::real_d minFrequencyStep = sensorElementWidth / (maxWavelength * 1e-6);
	math::real_d maxFrequencyStep = sensorElementWidth / (minWavelength * 1e-6);

	int estimatorSamples = 0;
	math::real_d sampleWindow = 0.0;
	math::real_d maxFreq = (sensorWidth / 2) / (minWavelength * 1e-6);

	sampleWindow = CftEstimator2D::getMinPhysicalDim(minFrequencyStep / settings.frequencyMultiplier, apertureRadius * 2);
	estimatorSamples = CftEstimator2D::getMinSamples(maxFreq, sampleWindow, MAX_SAMPLES);

	math::real dx = sampleWindow / estimatorSamples;
	math::real dy = sampleWindow / estimatorSamples;
	math::real cx = sampleWindow / 2;
	math::real cy = sampleWindow / 2;

	// Generate aperture function
	ComplexMap2D apertureFunction;
	apertureFunction.initialize(estimatorSamples, estimatorSamples);

	TextureNode dirtTexture;
	dirtTexture.loadFile("../../demos/textures/" "dirt.jpg", (math::real)2.2);

	IntersectionPoint a;
	for (int i = 0; i < estimatorSamples; i++) {
		math::real x = i * dx - cx;
		math::real u = (x * apertureRadius_inv) + 0.5;

		for (int j = 0; j < estimatorSamples; j++) {
			math::real y = j * dy - cy;
			math::real v = (y * apertureRadius_inv) + 0.5;

			a.m_textureCoodinates = math::loadVector(u, v);
			//math::real dirt = 1.0f;
			math::real dirt = math::getScalar(dirtTexture.sample(&a));

			if (aperture->filter(x, y)) {
				apertureFunction.set(math::Complex(1.0f * dirt, 0.0f), i, j);
			}
			else {
				apertureFunction.set(math::Complex(0.0f, 0.0f), i, j);
			}
		}
	}

	CftEstimator2D estimator;
	estimator.initialize(&apertureFunction, sampleWindow, sampleWindow);
	apertureFunction.destroy();

	math::real_d sdx = sensorElementWidth;
	math::real_d sdy = sensorElementWidth;
	math::real_d scx = sensorWidth / 2;
	math::real_d scy = sensorWidth / 2;
	math::real_d maxFreqX = estimator.getHorizontalFreqRange();
	math::real_d maxFreqY = estimator.getVerticalFreqRange();

	VectorMap2D temp;
	temp.initialize(outputResolution, outputResolution);
	for (int wavelength = minWavelength; wavelength <= maxWavelength; wavelength += wavelengthStep) {
		addLayer(&estimator, wavelength, &temp);
	}

	temp.roll(&m_diffractionPattern);
	temp.destroy();
	estimator.destroy();

	normalize();
}

void manta::FraunhoferDiffraction::destroy() {
	m_diffractionPattern.destroy();
}

manta::math::Vector manta::FraunhoferDiffraction::samplePattern(math::real dx, math::real dy) const {
	math::real u, v;

	u = dx / (m_physicalSensorWidth);
	v = dy / (m_physicalSensorWidth);

	u /= 64;
	v /= 64;

	if (u < 0) u += (math::real)1.0;
	if (v < 0) v += (math::real)1.0;

	return m_diffractionPattern.sample(u, v);
}

manta::math::Vector2 manta::FraunhoferDiffraction::getPerturbance(math::real u, math::real v) const {
	math::real dx, dy;

	dx = u * (m_physicalSensorWidth / 2);
	dy = v * (m_physicalSensorWidth / 2);

	return math::Vector2(dx, dy);
}

manta::math::Vector manta::FraunhoferDiffraction::getTotalFlux() const {
	int width = m_diffractionPattern.getWidth();
	int height = m_diffractionPattern.getHeight();

	math::Vector totalFlux = math::constants::Zero;
	math::real area = m_physicalSensorWidth * m_physicalSensorWidth;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			math::Vector fragment = m_diffractionPattern.get(i, j);
			math::real r = math::getX(fragment);
			math::real g = math::getY(fragment);
			math::real b = math::getZ(fragment);

			//totalFlux += 0.2126 * r + 0.7152 * g + 0.0722 * b;
			totalFlux = math::add(totalFlux, fragment);
		}
	}

	totalFlux = math::div(totalFlux, math::loadScalar(area));

	return totalFlux;
}

void manta::FraunhoferDiffraction::normalize() {
	math::Vector maxValue = m_diffractionPattern.get(0, 0);

	/*
	for (int i = 0; i < m_diffractionPattern.getWidth(); i++) {
		for (int j = 0; j < m_diffractionPattern.getHeight(); j++) {
			maxValue = math::componentMax(m_diffractionPattern.get(i, j), maxValue);
		}
	}*/

	math::Vector scale = math::div(math::constants::One, maxValue);
	scale = math::mask(scale, math::constants::MaskOffW);

	m_diffractionPattern.scale(scale);
}

manta::math::Vector manta::FraunhoferDiffraction::wavelengthToRgb(math::real_d wavelength) {
	math::real_d r = 0, g = 0, b = 0;

	if (wavelength >= 380 && wavelength <= 439) {
		r = -(wavelength - 440) / (440 - 380);
		g = 0.0;
		b = 1.0;
	}
	else if (wavelength >= 440 && wavelength <= 489) {
		r = 0.0;
		g = (wavelength - 440) / (490 - 440);
		b = 1.0;
	}
	else if (wavelength >= 490 && wavelength <= 509) {
		r = 0.0;
		g = 1.0;
		b = -(wavelength - 510) / (510 - 490);
	}
	else if (wavelength >= 510 && wavelength <= 579) {
		r = (wavelength - 510) / (580 - 510);
		g = 1.0;
		b = 0.0;
	}
	else if (wavelength >= 580 && wavelength <= 644) {
		r = 1.0;
		g = -(wavelength - 645) / (645 - 580);
		b = 0.0;
	}
	else if (wavelength >= 645 && wavelength <= 780) {
		r = 1.0;
		g = 0.0;
		b = 0.0;
	}
	else {
		r = 0.0;
		g = 0.0;
		b = 0.0;
	}

	math::real factor;
	if (wavelength >= 380 && wavelength <= 419) {
		factor = 0.3 + 0.7 * (wavelength - 380) / (420 - 380);
	}
	else if (wavelength >= 420 && wavelength <= 700) {
		factor = 1.0;
	}
	else if (wavelength >= 701 && wavelength <= 780) {
		factor = 0.3 + 0.7 * (780 - wavelength) / (780 - 700);
	}
	else {
		factor = 0.0;
	}

	return math::mul(math::loadVector(r, g, b), math::loadScalar(factor));
}

manta::math::real_d manta::FraunhoferDiffraction::blackBodyRadiation(math::real_d wavelength) {
	// Approximate for visible light wavelengths
	math::real_d w_2 = wavelength * wavelength;
	math::real_d w_m4 = 1 / (w_2 * w_2);

	return 800000000 * w_m4;
}

void manta::FraunhoferDiffraction::defaultSettings(Settings *settings) {
	settings->maxSamples = 4096;
	settings->maxWaveLength = 780;
	settings->minWaveLength = 380;
	settings->wavelengthStep = 5;
	settings->frequencyMultiplier = 3.0;
}

#include <iostream>

void manta::FraunhoferDiffraction::addLayer(const CftEstimator2D *estimator, math::real_d wavelength, VectorMap2D *target) {
	int res = target->getWidth();

	math::real_d sdx = m_sensorElementWidth;
	math::real_d sdy = m_sensorElementWidth;
	math::real_d scx = m_physicalSensorWidth / 2;
	math::real_d scy = m_physicalSensorWidth / 2;
	math::real_d maxFreqX = estimator->getHorizontalFreqRange();
	math::real_d maxFreqY = estimator->getVerticalFreqRange();

	math::Vector pureColor = wavelengthToRgb((math::real_d)wavelength);
	math::real blackBodyWeight = blackBodyRadiation(wavelength);

	for (int i = 0; i < res; i++) {
		math::real_d x = i * sdx - scx;
		math::real_d freq_x = (x / wavelength) * 1E6;

		if (abs(freq_x) > maxFreqX) continue;

		for (int j = 0; j < res; j++) {
			math::real_d y = j * sdy - scy;
			math::real_d freq_y = (y / wavelength) * 1E6;

			if (abs(freq_y) > maxFreqY) continue;

			math::Complex v = estimator->sample(freq_x, freq_y);
			v = v * v.conjugate();

			math::real_d intensity = v.r;
			math::Vector baseColor = target->get(i, j);

			target->set(math::add(math::mul(pureColor, math::loadScalar(intensity)), baseColor), i, j);
		}
	}
}
