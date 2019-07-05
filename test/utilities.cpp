#include <pch.h>

#include <utilities.h>

#include <image_byte_buffer.h>
#include <jpeg_writer.h>
#include <sdl_compilation_error.h>
#include <sdl_error_list.h>

void writeToJpeg(const RealMap2D *scalarMap, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	scalarMap->fillByteBuffer(&byteBuffer, true);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void writeToJpeg(const VectorMap2D *vectorMap, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	vectorMap->fillByteBuffer(&byteBuffer, true);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void writeToJpeg(const ImagePlane *plane, const std::string &fname) {
	ImageByteBuffer byteBuffer;
	byteBuffer.initialize(plane, true);

	JpegWriter writer;
	writer.setQuality(95);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

void writeToJpeg(const ComplexMap2D *plane, const std::string &fname, Margins *margins) {
	ImageByteBuffer byteBuffer;
	plane->fillByteBuffer(&byteBuffer, margins);

	JpegWriter writer;
	writer.setQuality(100);
	writer.write(&byteBuffer, fname.c_str());

	byteBuffer.free();
}

bool findError(const SdlErrorList *errorList, const SdlErrorCode_struct &errorCode, int line, 
										const SdlCompilationUnit *unit, bool instantiationError) {
	int errorCount = errorList->getErrorCount();

	for (int i = 0; i < errorCount; i++) {
		SdlCompilationError *error = errorList->getCompilationError(i);
		if (unit == nullptr || error->getCompilationUnit() == unit) {
			if (error->getErrorCode().code == errorCode.code && error->getErrorCode().stage == errorCode.stage) {
				if (line == -1 || error->getErrorLocation()->lineStart == line) {
					if (error->isInstantiationError() == instantiationError) {
						return true;
					}
				}
			}
		}
	}

	return false;
}
