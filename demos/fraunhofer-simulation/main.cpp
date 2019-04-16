#include <manta_math.h>
#include <standard_allocator.h>
#include <signal_processing.h>
#include <complex_map_2d.h>
#include <vector_map_2d.h>
#include <fraunhofer_diffraction.h>
#include <cft_estimator_2d.h>
#include <circular_aperture.h>
#include <polygonal_aperture.h>
#include <image_byte_buffer.h>
#include <jpeg_writer.h>
#include <texture_node.h>
#include <intersection_point.h>
#include <spectrum.h>
#include <cmf_table.h>
#include <rgb_space.h>

#include <chrono>
#include <sstream>

#define CMF_PATH "../../demos/cmfs/"
#define WORKSPACE_PATH "../../workspace/"
#define TMP_PATH (WORKSPACE_PATH "tmp/")
#define TEXTURE_PATH "../../demos/textures/"

using namespace manta;

math::Vector adjust(const math::Vector &color, math::real factor) {
	return math::mul(color, math::loadScalar(factor));
}

math::Vector wavelengthToRgb(math::real_d wavelength) {
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

void writeToJpeg(const ComplexMap2D *plane, const std::string &fname, Margins *margins = nullptr) {
	ImageByteBuffer byteBuffer;
	plane->fillByteBuffer(&byteBuffer, margins);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void writeToJpeg(const VectorMap2D *vectorMap, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	vectorMap->fillByteBuffer(&byteBuffer);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void addLayerRgb(VectorMap2D *target,
	CmfTable *table,
	Spectrum *sourceSpectrum,
	const CftEstimator2D &estimator,
	math::real_d sensorElementWidth,
	math::real_d sensorWidth, int startWavelength, int endWavelength, int wavelengthStep) {
	int res = target->getWidth();

	math::real_d sdx = sensorElementWidth;
	math::real_d sdy = sensorElementWidth;
	math::real_d scx = sensorWidth / 2;
	math::real_d scy = sensorWidth / 2;
	math::real_d maxFreqX = estimator.getHorizontalFreqRange();
	math::real_d maxFreqY = estimator.getVerticalFreqRange();

	int steps = (endWavelength - startWavelength) / wavelengthStep;
	Spectrum spectrum, spectrumTmp;
	spectrum.initialize(steps + 1, (math::real)startWavelength, (math::real)endWavelength, nullptr);
	spectrumTmp.initialize(steps + 1, (math::real)startWavelength, (math::real)endWavelength, nullptr);

	for (int i = 0; i < res; i++) {
		math::real_d x = i * sdx - scx;

		for (int j = 0; j < res; j++) {
			math::real_d y = j * sdy - scy;

			if (i == 512 && j == 512) {
				int a = 0;
			}

			math::Vector color = math::constants::Zero;

			for (int wavelength = startWavelength; wavelength <= endWavelength; wavelength += wavelengthStep) {
				math::real_d freq_x = ((x * 1E6) / wavelength);
				math::real_d freq_y = ((y * 1E6) / wavelength);

				if (abs(freq_y) > maxFreqY) continue;
				if (abs(freq_x) > maxFreqX) continue;

				math::Complex v = estimator.sample(freq_x, freq_y, sdx * 1E6 / wavelength);
				v = v * v.conjugate();

				math::real src = sourceSpectrum->getValueContinuous(wavelength);
				math::Vector pureColor = wavelengthToRgb(wavelength); //table->pureToRgb(wavelength);
				pureColor = table->pureToRgb(wavelength);
				math::real weight = src * v.r / (wavelength * wavelength);
				color = math::add(math::mul(pureColor, math::loadScalar(weight)), color);
			}

			target->set(color, i, j);
		}
		std::cout << "Row " << i << " / " << res << std::endl;
	}

	std::cout << "Finding maximum luminance" << std::endl;
	math::Vector componentMax = math::constants::Zero;
	for (int i = 0; i < res; i++) {
		for (int j = 0; j < res; j++) {
			math::Vector rgbColor = target->get(i, j);
			componentMax = math::componentMax(rgbColor, componentMax);
		}
	}
	//maxY = 0.00001;

	//std::cout << "Max Y: " << maxY << std::endl;
	std::cout << "Normalizing..." << std::endl;

	// Normalize
	for (int i = 0; i < res; i++) {
		for (int j = 0; j < res; j++) {
			if (i == 250 && j == 244) {
				int a = 0;
			}

			math::Vector rgbColor = target->get(i, j);
			rgbColor = math::div(rgbColor, componentMax);

			target->set(rgbColor, i, j);
		}
	}
}

void addLayer(VectorMap2D *target, 
	CmfTable *table, 
	Spectrum *sourceSpectrum,
	const CftEstimator2D &estimator, 
	math::real_d sensorElementWidth, 
	math::real_d sensorWidth, int startWavelength, int endWavelength, int wavelengthStep) {
	int res = target->getWidth();

	constexpr int SAMPLES = 10;

	math::real_d sdx = sensorElementWidth;
	math::real_d sdy = sensorElementWidth;
	math::real_d scx = sensorWidth / 2;
	math::real_d scy = sensorWidth / 2;
	math::real_d maxFreqX = estimator.getHorizontalFreqRange();
	math::real_d maxFreqY = estimator.getVerticalFreqRange();

	int steps = (endWavelength - startWavelength) / wavelengthStep;
	Spectrum spectrum, spectrumTmp;
	spectrum.initialize(steps + 1, (math::real)startWavelength, (math::real)endWavelength, nullptr);
	spectrumTmp.initialize(steps + 1, (math::real)startWavelength, (math::real)endWavelength, nullptr);

	for (int i = 0; i < res; i++) {
		math::real_d x = i * sdx - scx;

		for (int j = 0; j < res; j++) {
			math::real_d y = j * sdy - scy;

			if (i == 512 && j == 512) {
				int a = 0;
			}

			spectrum.clear();

			for (int wavelength = startWavelength; wavelength <= endWavelength; wavelength += wavelengthStep) {
				int samples = 0;
				for (int s = 0; s < SAMPLES; s++) {
					math::real_d freqSpace = sdx * 1E6 / wavelength;
					math::real_d freq_x = ((x * 1E6) / wavelength);
					math::real_d freq_y = ((y * 1E6) / wavelength);

					math::real randX = math::uniformRandom() - 0.5;
					math::real randY = math::uniformRandom() - 0.5;

					freq_x += randX * freqSpace;
					freq_y += randY * freqSpace;

					if (abs(freq_y) > maxFreqY) continue;
					if (abs(freq_x) > maxFreqX) continue;

					math::Complex v = estimator.sample(freq_x, freq_y, sdx * 1E6 / wavelength);
					v = v * v.conjugate();

					math::real src = sourceSpectrum->getValueContinuous(wavelength);
					int wave_i = (int)((wavelength - startWavelength) / wavelengthStep + 0.5);
					math::real prev = spectrum.getValueDiscrete(wave_i);
					spectrum.set(wave_i, prev + src * v.r / (wavelength * wavelength));
				}
			}

			ColorXyz xyzColor = table->spectralToXyz(&spectrum);
			target->set(math::loadVector(xyzColor.x, xyzColor.y, xyzColor.z), i, j);
		}
		std::cout << "Row " << i << " / " << res << std::endl;
	}

	std::cout << "Finding maximum luminance" << std::endl;
	math::real_d maxY = 0.0;
	math::Vector maxCol;
	for (int i = 0; i < res; i++) {
		for (int j = 0; j < res; j++) {
			math::Vector xyzColor = target->get(i, j);
			if (math::getY(xyzColor) > maxY) {
				maxY = math::getY(xyzColor);
				maxCol = xyzColor;
			}
		}
	}
	//maxY = 0.00001;

	std::cout << "Max Y: " << maxY << std::endl;
	std::cout << "Normalizing..." << std::endl;

	math::Vector d65 = math::loadVector(0.95047f, 1.0f, 1.08883f);
	math::Vector norm = math::div(math::constants::One, math::loadScalar(maxY));

	// Normalize
	for (int i = 0; i < res; i++) {
		for (int j = 0; j < res; j++) {
			if (i == 250 && j == 244) {
				int a = 0;
			}

			math::Vector xyzColor = target->get(i, j);
			xyzColor = math::mul(xyzColor, norm);

			ColorXyz xyzColorT = ColorXyz(math::getX(xyzColor), math::getY(xyzColor), math::getZ(xyzColor));
			math::Vector rgbColor = table->xyzToRgb(xyzColorT);

			target->set(rgbColor, i, j);
		}
	}

	spectrumTmp.destroy();
	spectrum.destroy();
}

math::real blackBodyRadiation(math::real_d wavelength) {
	// Approximate for visible light wavelengths

	math::real_d w_2 = wavelength * wavelength;
	math::real_d w_m4 = 1 / (w_2 * w_2);

	return 800000000 * w_m4;
}

int main() {
	constexpr int SENSOR_RESOLUTION = 1024;
	constexpr int MAX_SAMPLES = 4096;

	constexpr math::real_d apertureRadius = 5.5f;
	constexpr math::real_d sensorWidth = 0.05f; // mm
	constexpr math::real_d sensorElementWidth = sensorWidth / SENSOR_RESOLUTION;

	constexpr math::real_d minWavelength = 380e-6;
	constexpr math::real_d maxWavelength = 780e-6;

	math::real_d minFrequencyStep = sensorElementWidth / maxWavelength;
	math::real_d maxFrequencyStep = sensorElementWidth / minWavelength;

	int estimatorSamples = 0;
	math::real_d sampleWindow = 0.0;
	math::real_d maxFreq = (sensorWidth / 2) / minWavelength;

	sampleWindow = CftEstimator2D::getMinPhysicalDim(minFrequencyStep / 3, apertureRadius * 2);
	sampleWindow = 50.0f;
	estimatorSamples = CftEstimator2D::getMinSamples(maxFreq, sampleWindow, MAX_SAMPLES);
	estimatorSamples = MAX_SAMPLES;

	TextureNode dirtTexture;
	dirtTexture.loadFile(TEXTURE_PATH "dirt.jpg", (math::real)2.2);

	ComplexMap2D apertureFunction;
	PolygonalAperture aperture;
	aperture.setRadius(apertureRadius);
	aperture.initialize(5, math::constants::PI / 2);

	math::real_d fs_x = estimatorSamples / sampleWindow;
	math::real_d fs_y = estimatorSamples / sampleWindow;

	// Sample the aperture function
	apertureFunction.initialize(estimatorSamples, estimatorSamples);

	math::real dx = sampleWindow / estimatorSamples;
	math::real dy = sampleWindow / estimatorSamples;
	math::real cx = sampleWindow / 2;
	math::real cy = sampleWindow / 2;

	int maxResolution = (int)((CftEstimator2D::getFreqRange(estimatorSamples, sampleWindow) * maxWavelength + sensorWidth / 2) / sensorElementWidth);

	std::cout << "Parameters =====================" << std::endl;
	std::cout << "Spatial extent: " << sampleWindow << " mm" << std::endl;
	std::cout << "Estimator samples: " << estimatorSamples << std::endl;
	std::cout << "Frequency step: " << CftEstimator2D::getFreqStep(sampleWindow) << " 1/mm" << std::endl;
	std::cout << "Frequency range: " << CftEstimator2D::getFreqRange(estimatorSamples, sampleWindow) << " 1/mm" << std::endl;
	std::cout << "Minimum sensor frequency step (380 nm light): " << minFrequencyStep << "1/mm" << std::endl;
	std::cout << "Max resolution: " << maxResolution << " px" << std::endl;

	IntersectionPoint a;

	std::cout << "Generating aperture function" << std::endl;
	for (int i = 0; i < estimatorSamples; i++) {
		math::real x = i * dx - cx;

		for (int j = 0; j < estimatorSamples; j++) {
			math::real y = j * dy - cy;

			if (aperture.filter(x, y)) {
				math::real u = (x / (2 * aperture.getRadius())) + 0.5;
				math::real v = (y / (2 * aperture.getRadius())) + 0.5;

				a.m_textureCoodinates = math::loadVector(u, v);
				math::real dirt = math::getScalar(dirtTexture.sample(&a));
				//dirt = 1.0;

				apertureFunction.set(math::Complex(1.0f * dirt, 0.0f), i, j);
			}
			else {
				apertureFunction.set(math::Complex(0.0f, 0.0f), i, j);
			}

			//if (abs(x) < 0.01 && abs(y) < 0.01) {
			//	apertureFunction.set(math::Complex(0.0f, 0.0f), i, j);
			//}
		}
	}

	std::cout << "Estimating Fraunhofer diffraction" << std::endl;
	auto startTime = std::chrono::system_clock::now();

	writeToJpeg(&apertureFunction, std::string(TMP_PATH) + "aperture_function.jpg");
	
	CftEstimator2D estimator;
	estimator.initialize(&apertureFunction, sampleWindow, sampleWindow);
	apertureFunction.destroy();

	auto endTime = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = endTime - startTime;

	std::cout << "Calculation time: " << diff.count() << " s" << std::endl;

	std::cout << "Processing" << std::endl;
	ComplexMap2D viewable;
	estimator.getApproximation()->roll(&viewable);
	viewable.magnitude();
	viewable.multiply(math::Complex(1.0f, 0.0f));

	std::cout << "Down sampling to sensor size" << std::endl;
	math::real_d sdx = sensorElementWidth;
	math::real_d sdy = sensorElementWidth;
	math::real_d scx = sensorWidth / 2;
	math::real_d scy = sensorWidth / 2;
	math::real_d maxFreqX = estimator.getHorizontalFreqRange();
	math::real_d maxFreqY = estimator.getVerticalFreqRange();

	// Fill sensor array
	VectorMap2D result;
	result.initialize(SENSOR_RESOLUTION, SENSOR_RESOLUTION);

	/*
	for (int l = 380; l <= 780; l += 2) {
		math::real blackBodyWeight = 1.0; // blackBodyRadiation((math::real_d)l);

		std::cout << "Adding wavelength " << l << " nm | weight = " << blackBodyWeight << std::endl;
		addLayer(&result, estimator, sensorElementWidth, sensorWidth, l, 
			math::mul(
				math::normalize(wavelengthToRgb((math::real_d)l)), 
				math::loadScalar(blackBodyWeight)));
	}*/
	CmfTable table;
	table.loadCsv(CMF_PATH "xyz_cmf_31.csv");
	Spectrum source;
	source.loadCsv(CMF_PATH "d65_lighting.csv");

	addLayer(&result, &table, &source, estimator, sensorElementWidth, sensorWidth, 380, 780, 2);
	//addLayerRgb(&result, &table, &source, estimator, sensorElementWidth, sensorWidth, 380, 780, 2);
	//addLayer(&result, &table, &source, estimator, sensorElementWidth, sensorWidth, 400, 405, 5);

	/*
	for (int i = 380; i < 780; i++) {
		addLayer(&result, &table, &source, estimator, sensorElementWidth, sensorWidth, i, i+1, 1);

		std::stringstream ss;
		ss << "by-wavelength/plane_" << i << "_nm.jpg";
		result.scale(math::loadScalar(1));
		writeToJpeg(&result, std::string(TMP_PATH) + ss.str());
	}*/
	source.destroy();

	/*
	math::real_d totalFlux = 0;
	for (int i = 0; i < result.getWidth(); i++) {
		for (int j = 0; j < result.getHeight(); j++) {
			math::Vector fragment = result.get(i, j);
			math::real r = math::getX(fragment);
			math::real g = math::getY(fragment);
			math::real b = math::getZ(fragment);

			totalFlux += 0.2126 * r + 0.7152 * g + 0.0722 * b;
		}
	}*/

	//totalFlux /= (sensorWidth * sensorWidth);

	//std::cout << "Total flux: " << totalFlux << std::endl;

	//math::real maxMag = result.getMaxMagnitude();
	//result.scale(math::loadScalar((1 / maxMag) * ::sqrt(3)));
	//result.scale(math::loadScalar(1 / totalFlux));
	result.scale(math::loadScalar(10000));
	//result.applyGamma(0.8f);

	std::cout << "Writing to file" << std::endl;
	writeToJpeg(&viewable, std::string(TMP_PATH) + "cft_approx.jpg");
	writeToJpeg(&result, std::string(TMP_PATH) + "sensing_plane.jpg");
	viewable.destroy();
	result.destroy();

	estimator.destroy();
	apertureFunction.destroy();
	//aperture.destroy();
}