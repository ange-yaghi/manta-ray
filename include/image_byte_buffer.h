#ifndef IMAGE_BYTE_BUFFER_H
#define IMAGE_BYTE_BUFFER_H

#include <manta_math.h>

namespace manta {

	// Forward declarations
	class SceneBuffer;

	class ImageByteBuffer {
	public:
		struct Color {
			unsigned char r, g, b, a;
		};

	public:
		ImageByteBuffer();
		~ImageByteBuffer();

		void initialize(SceneBuffer *sceneBuffer);
		void initialize(unsigned char *buffer, int width, int height, int pitch);
		void free();

		unsigned char *getBuffer() const { return m_buffer; }

		int getPitch() const { return m_pitch; }
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

	protected:
		void setPixel(int row, int column, const Color &c);
		void convertToColor(const math::Vector &v, Color *c) const;

		int m_pitch;
		int m_width;
		int m_height;

		unsigned char *m_buffer;
	};

} /* namespace manta */

#endif /* IMAGE_BYTE_BUFFER_H */
