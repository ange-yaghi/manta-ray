#include <pch.h>

#include <utilities.h>

#include <image_byte_buffer.h>
#include <jpeg_writer.h>

void writeToJpeg(const ScalarMap2D *scalarMap, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	scalarMap->fillByteBuffer(&byteBuffer);

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

void writeToJpeg(const ImagePlane *plane, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	byteBuffer.initialize(plane);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void writeToJpeg(const ComplexMap2D *plane, const std::string &fname, bool useMargin) {
	ImageByteBuffer byteBuffer;
	plane->fillByteBuffer(&byteBuffer, useMargin);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}
