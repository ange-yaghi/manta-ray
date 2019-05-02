#ifndef IMAGE_PLANE_H
#define IMAGE_PLANE_H

#include <manta_math.h>

namespace manta {

	// Forward declarations
	class Convolution;

	class ImagePlane {
	public:
		ImagePlane();
		~ImagePlane();

		void initialize(int width, int height, math::real physicalWidth, math::real physicalHeight);
		void destroy();

		bool checkPixel(int x, int y) const;
		inline void set(const math::Vector &v, int x, int y);
		inline math::Vector sample(int x, int y) const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		bool isInitialized() const { return m_buffer != nullptr; }

		void copyFrom(const ImagePlane *source);
		void createEmptyFrom(const ImagePlane *source);
		void clear(const math::Vector &v = math::constants::Zero);

		const math::Vector *getBuffer() const { return m_buffer; }

		// ----- Analysis and Manipulation -----

		// Get max intensity in the scene buffer
		math::real getMax() const;

		// Get min intensity in the scene buffer
		math::real getMin() const;

		// Get the average value in the scene buffer
		math::Vector getAverage() const;

		// Apply gamma
		void applyGammaCurve(math::real gamma);

		void add(const ImagePlane *b);
		
		// Scale all values in the scene buffer
		void scale(math::real scale);

	protected:
		math::real m_physicalWidth;
		math::real m_physicalHeight;

		int m_width;
		int m_height;
		math::Vector *m_buffer;
	};

} /* namespace manta */

#endif /* IMAGE_PLANE_H */
