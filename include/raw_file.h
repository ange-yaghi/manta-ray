#ifndef RAW_FILE_H
#define RAW_FILE_H

namespace manta {

	class SceneBuffer;

	class RawFile {
	public:
		struct MainHeader {
			unsigned int magicWord;
			unsigned int version;
			unsigned int dataHeaderSize;
		};

		struct DataHeader_v1 {
			int width;
			int height;
			int precision; // Size of the floating point type (either double or float) used
			unsigned int pixelDataSize;
		};

		struct FloatPixel_v1 {
			float r;
			float g;
			float b;
		};

		struct DoublePixel_v1 {
			double r;
			double g;
			double b;
		};

	public:
		static const int VERSION = 1;
		static const int MAGIC_WORD = 0xA50E;

	public:
		RawFile();
		~RawFile();

		bool writeRawFile(const char *fname, const SceneBuffer *buffer) const;
		bool readRawFile(const char *fname, SceneBuffer *buffer) const;

	protected:
		void *generatePixelArray(const SceneBuffer *buffer, int version, int *size) const;
		void *generateEmptyPixelArray(void *dataHeader, int version, int *pixelDataSize) const ;
		bool readPixelArray(void *dataHeader, void *pixelData, SceneBuffer *buffer, int version) const;

		void *generateDataHeader(const SceneBuffer *buffer, int version, int *size) const;
	};

} /* namespace manta */

#endif /* RAW_WRITER_H */
