#include <pch.h>

#include <jpeg_writer.h>

#include <image_byte_buffer.h>

using namespace manta;

TEST(JpegTests, JpegBasicTest) {
	JpegWriter jpegWriter;

	int width = 100, height = 100;

	unsigned char *buffer = new unsigned char[width * height * 4];

	// Write sample image data
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			buffer[i * 4 + j * (width * 4)] = (int)((i / (float)width) * 255);
			buffer[i * 4 + j * (width * 4) + 1] = (int)((i / (float)width) * 255);
			buffer[i * 4 + j * (width * 4) + 2] = (int)((i / (float)width) * 255);
			buffer[i * 4 + j * (width * 4) + 3] = 0; // Unused
		}
	}

	ImageByteBuffer byteBuffer;
	byteBuffer.initialize(buffer, width, height, 4);

	bool result = jpegWriter.write(&byteBuffer, "test.jpg");
	EXPECT_TRUE(result);

	// Delete the file
	remove("test.jpg");

	byteBuffer.free();
}
