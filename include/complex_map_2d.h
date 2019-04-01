#ifndef COMPLEX_MAP_2D_H
#define COMPLEX_MAP_2D_H

#include <manta_math.h>

#include <complex_math.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class VectorMap2D;
	class ScalarMap2D;
	class ImagePlane;

	class ComplexMap2D {
	public:
		struct Margins {
			int left;
			int width;
			int top;
			int height;
		};

	public:
		ComplexMap2D();
		~ComplexMap2D();

		void initialize(int width, int height, math::real value = (math::real)0.0);
		void setMargins(int left, int top, int width, int height);
		void destroy();

		math::Complex get(int u, int v) const;
		void set(const math::Complex &value, int u, int v);

		void fillByteBuffer(ImageByteBuffer *target, bool followMargins = false) const;

		void fft(ComplexMap2D *target) const;
		void inverseFft(ComplexMap2D *target) const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		void resizeSafe(ComplexMap2D *target) const;
		void resize(int width, int height, ComplexMap2D *target) const;

		// Binary operations
		void multiply(const ComplexMap2D *b);
		void copy(const ComplexMap2D *b);
		void copy(const ScalarMap2D *b);
		void copy(const ImagePlane *b, int channel);
		void copy(const VectorMap2D *b, int channel);

	protected:
		math::Complex *m_data;
		Margins m_margins;

		int m_width;
		int m_height;
	};

} /* namespace manta */

#endif /* COMPLEX_MAP_2D_H */
