#ifndef JPEG_WRITER_H
#define JPEG_WRITER_H

#include <math.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;

	class JpegWriter {
	public:
		static const int DEFAULT_QUALITY = 95;

	public:
		JpegWriter();
		~JpegWriter();

		bool write(ImageByteBuffer *buffer, const char *fileName);

		void setQuality(int quality) { m_quality = quality; }
		int getQuality() const { return m_quality; }

	protected:
		int m_quality;
	};

} /* namespace pft */

#endif /* JPEG_WRITER_H */
