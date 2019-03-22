#include <jpeg_writer.h>

#include <image_byte_buffer.h>

#include <stdio.h>
#include <turbojpeg.h>

#ifdef _WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

manta::JpegWriter::JpegWriter() {
	m_quality = DEFAULT_QUALITY;
}

manta::JpegWriter::~JpegWriter() {

}

bool manta::JpegWriter::write(ImageByteBuffer *buffer, const char *fileName) {
	tjhandle tjInstance = nullptr;

	// Parameters
	int outSubsamp, outQual;
	int pixelFormat;
	int flags;

	tjInstance = tjInitCompress();

	if (tjInstance == nullptr) return false;

	outSubsamp = TJSAMP_444;
	outQual = m_quality;
	pixelFormat = TJPF_RGBX;
	flags = 0;

	unsigned char *jpegBuffer = nullptr;
	unsigned long jpegSize;
	int result = tjCompress2(tjInstance, buffer->getBuffer(), buffer->getWidth(), 0, buffer->getHeight(), 
		pixelFormat, &jpegBuffer, &jpegSize, outSubsamp, outQual, flags);

	if (result < 0) return false;

	// Write to disk
	FILE *jpegFile = nullptr;
	result = fopen_s(&jpegFile, fileName, "wb");
	if (jpegFile == nullptr || result != 0) {
		return false;
	}

	result = fwrite((const void *)jpegBuffer, jpegSize, 1, jpegFile);
	if (result < 0) return false;

	tjDestroy(tjInstance);
	fclose(jpegFile);
	tjFree(jpegBuffer);

	return true;
}
