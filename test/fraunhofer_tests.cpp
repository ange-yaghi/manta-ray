#include <pch.h>
#include "utilities.h"

#include <fraunhofer_diffraction.h>
#include <image_plane.h>
#include <circular_aperture.h>
#include <square_aperture.h>
#include <polygonal_aperture.h>
#include <image_byte_buffer.h>
#include <jpeg_writer.h>

using namespace manta;

/*
TEST(FraunhoferTests, FraunhoferFullTest) {
	//CircularAperture ap;
	//ap.setRadius(0.25f);

	PolygonalAperture ap;
	ap.initialize(8);
	ap.setRadius(1.0f);

	FraunhoferDiffraction conv;
	conv.generate(&ap, 512);

	constexpr int imageWidth = 1920 / 4;
	constexpr int imageHeight = 1080 / 4;
	constexpr int width = 512;

	ScalarMap2D image;
	image.initialize(imageWidth, imageHeight, 0.5f);

	// Single pulse at center
	int pulseWidth = 20;
	int cx = imageWidth / 2;
	int cy = imageHeight / 2;

	for (int i = -pulseWidth; i < pulseWidth; i++) {
		for (int j = -pulseWidth; j < pulseWidth; j++) {
			image.set(0.00001f, cx + i, cy + j);
		}
	}

	image.set(0.5f, cx, cy);

	Margins margins;
	ComplexMap2D imageC, imageSafe;
	imageC.copy(&image);
	image.destroy();

	imageC.resizeSafe(&imageSafe, &margins);
	imageC.destroy();

	ComplexMap2D diffractionPattern, diffPatternFit;
	diffractionPattern.copy(conv.getDiffractionPattern(), 0);
	diffractionPattern.resize(imageSafe.getWidth(), imageSafe.getHeight(), &diffPatternFit);

	ComplexMap2D diffractionF, imageSafeF, output;
	diffPatternFit.fft(&diffractionF);
	imageSafe.fft(&imageSafeF);

	imageSafeF.multiply(&diffractionF);
	imageSafeF.inverseFft(&output);

	writeToJpeg(&imageSafe, std::string(TMP_PATH) + "safe.jpg", false);
	writeToJpeg(&diffPatternFit, std::string(TMP_PATH) + "diffraction.jpg", false);
	writeToJpeg(&output, std::string(TMP_PATH) + "output.jpg", &margins);

	imageSafe.destroy();
	diffractionPattern.destroy();
	diffPatternFit.destroy();
	diffractionF.destroy();
	imageSafeF.destroy();
	output.destroy();
	ap.destroy();

	conv.destroy();
}

TEST(FraunhoferTests, FraunhoferPolygonTest) {
	PolygonalAperture ap;
	ap.initialize(5);
	ap.setRadius(1.0f);

	FraunhoferDiffraction conv;
	conv.generate(&ap, 512);

	constexpr int width = 1000;

	ImagePlane plane;
	plane.initialize(width, width, 1.0f, 1.0f);

	// Single pulse at 50, 50
	plane.clear();
	if (width == 100) {
		plane.set(math::loadScalar(500.f), width / 2, width / 2);
	}
	else if (width == 1000) {
		int pulseWidth = 1;
		int cx = width / 2;
		int cy = width / 2;

		for (int i = -pulseWidth; i < pulseWidth; i++) {
			for (int j = -pulseWidth; j < pulseWidth; j++) {
				plane.set(math::loadScalar(5000.f), cx + i, cy + j);
			}
		}
	}

	ImagePlane convolution;
	plane.convolution(&conv, &convolution);

	writeToJpeg(conv.getDiffractionPattern(), std::string(TMP_PATH) + "diffraction_5.jpg");
	writeToJpeg(&convolution, std::string(TMP_PATH) + "convolution_5.jpg");

	conv.destroy();
	convolution.destroy();
	plane.destroy();
	ap.destroy();
}

TEST(FraunhoferTests, FraunhoferSanityTest) {
	PolygonalAperture ap;
	ap.initialize(5);
	ap.setRadius(1.0f);

	//CircularAperture ap;
	//ap.setRadius(1.0f);

	constexpr int size = 1024;

	FraunhoferDiffraction conv;
	conv.generate(&ap, size);

	std::stringstream ss;
	ss << TMP_PATH << "diffraction_5_" << size << ".jpg";

	ComplexMap2D result, resultRolled;
	result.copy(conv.getDiffractionPattern(), 0);
	result.roll(&resultRolled);

	writeToJpeg(&resultRolled, ss.str());

	conv.destroy();
	ap.destroy();
	result.destroy();
	resultRolled.destroy();
}
*/