#include <raw_file.h>

#include <scene_buffer.h>
#include <manta_math.h>

#include <fstream>

manta::RawFile::RawFile() {
}

manta::RawFile::~RawFile() {
}

bool manta::RawFile::writeRawFile(const char *fname, const SceneBuffer *buffer) const {
	int dataHeaderSize = 0;
	void *dataHeader = generateDataHeader(buffer, VERSION, &dataHeaderSize);

	if (dataHeader == nullptr) {
		return false;
	}

	int pixelArraySize = 0;
	void *pixelArray = generatePixelArray(buffer, VERSION, &pixelArraySize);

	if (pixelArray == nullptr) {
		return false;
	}

	// Write the file
	MainHeader mainHeader;
	mainHeader.dataHeaderSize = sizeof(DataHeader_v1);
	mainHeader.magicWord = MAGIC_WORD;
	mainHeader.version = VERSION;

	std::ofstream file(fname, std::ios::binary);
	file.write((const char *)&mainHeader, sizeof(MainHeader));
	file.write((const char *)dataHeader, dataHeaderSize);
	file.write((const char *)pixelArray, pixelArraySize);
	file.close();

	delete dataHeader;
	delete[] pixelArray;

	return true;
}

bool manta::RawFile::readRawFile(const char *fname, SceneBuffer *buffer) const {
	std::ifstream file(fname, std::ios::binary);

	MainHeader mainHeader;
	file.read((char *)&mainHeader, sizeof(MainHeader));

	if (mainHeader.magicWord != MAGIC_WORD) {
		// File is not the correct type
		file.close();
		return false;
	}

	if (mainHeader.version > VERSION) {
		// File is too new
		file.close();
		return false;
	}

	void *dataHeader = malloc(mainHeader.dataHeaderSize);
	file.read((char *)dataHeader, mainHeader.dataHeaderSize);

	int pixelDataSize;
	void *pixelData = generateEmptyPixelArray(dataHeader, mainHeader.version, &pixelDataSize);

	if (pixelData == nullptr) {
		free((void *)dataHeader);
		file.close();

		return false;
	}

	file.read((char *)pixelData, pixelDataSize);
	bool result = readPixelArray(dataHeader, pixelData, buffer, mainHeader.version);

	if (!result) {
		buffer->destroy();
	}

	free((void *)dataHeader);
	free((void *)pixelData);
	file.close();

	return result;
}

void *manta::RawFile::generatePixelArray(const SceneBuffer *buffer, int version, int *size) const {
	if (version == 0x1) {
		size_t s = sizeof(math::real);
		int width = buffer->getWidth();
		int height = buffer->getHeight();
		if (s == 4) {
			// Single precision floating point
			FloatPixel_v1 *v = new FloatPixel_v1[width * height];

			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					FloatPixel_v1 *px = &v[y * width + x];
					math::Vector value = buffer->sample(x, y);
					px->r = math::getX(value);
					px->g = math::getY(value);
					px->b = math::getZ(value);
				}
			}

			*size = sizeof(FloatPixel_v1) * width * height;
			return (void *)v;
		}
		else if (s == 8) {
			// Double precision floating point
			DoublePixel_v1 *v = new DoublePixel_v1[width * height];

			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					DoublePixel_v1 *px = &v[y * width + x];
					math::Vector value = buffer->sample(x, y);
					px->r = math::getX(value);
					px->g = math::getY(value);
					px->b = math::getZ(value);
				}
			}

			*size = sizeof(DoublePixel_v1) * width * height;
			return (void *)v;
		}
		else return nullptr;
	}
	else return nullptr;
}

void *manta::RawFile::generateEmptyPixelArray(void *dataHeader, int version, int *pixelDataSize) const {
	if (version == 0x1) {
		DataHeader_v1 *header = (DataHeader_v1 *)dataHeader;
		*pixelDataSize = header->pixelDataSize;
		return malloc(header->pixelDataSize);
	}
	else return nullptr;
}

bool manta::RawFile::readPixelArray(void *dataHeader, void *pixelData, SceneBuffer *buffer, int version) const {
	if (version == 0x1) {
		DataHeader_v1 *header = (DataHeader_v1 *)dataHeader;
		buffer->initialize(header->width, header->height);

		if (header->precision == 4) {
			FloatPixel_v1 *v = (FloatPixel_v1 *)pixelData;
			for (int x = 0; x < header->width; x++) {
				for (int y = 0; y < header->height; y++) {
					FloatPixel_v1 *px = &v[y * header->width + x];
					math::Vector value = math::loadVector(px->r, px->g, px->b);
					buffer->set(value, x, y);
				}
			}
			return true;
		}
		else if (header->precision == 8) {
			DoublePixel_v1 *v = (DoublePixel_v1 *)pixelData;
			for (int x = 0; x < header->width; x++) {
				for (int y = 0; y < header->height; y++) {
					DoublePixel_v1 *px = &v[y * header->width + x];
					math::Vector value = math::loadVector(px->r, px->g, px->b);
					buffer->set(value, x, y);
				}
			}
			return true;
		}
		else return false;
	}
	else return false;
}

void *manta::RawFile::generateDataHeader(const SceneBuffer *buffer, int version, int *size) const {
	if (version == 0x1) {
		DataHeader_v1 *dataHeader = new DataHeader_v1;
		dataHeader->width = buffer->getWidth();
		dataHeader->height = buffer->getHeight();
		dataHeader->precision = sizeof(math::real);

		if (dataHeader->precision == 4) {
			dataHeader->pixelDataSize = sizeof(FloatPixel_v1) * buffer->getWidth() * buffer->getHeight();
		}
		else if (dataHeader->precision == 8) {
			dataHeader->pixelDataSize = sizeof(DoublePixel_v1) * buffer->getWidth() * buffer->getHeight();
		}

		*size = sizeof(DataHeader_v1);
		return (void *)dataHeader;
	}
	else return nullptr;
}
