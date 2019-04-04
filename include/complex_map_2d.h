#ifndef COMPLEX_MAP_2D_H
#define COMPLEX_MAP_2D_H

#include <manta_math.h>

#include <complex_math.h>
#include <margins.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class VectorMap2D;
	class ScalarMap2D;
	class ImagePlane;

	class ComplexMap2D {
	public:
		ComplexMap2D();
		~ComplexMap2D();

		void initialize(int width, int height, math::real value = (math::real)0.0);
		void destroy();

		// Sample the map as though it were discrete
		math::Complex get(int u, int v) const;
		void set(const math::Complex &value, int u, int v);

		// Continuous approximation sampling
		// NOTE: inputs refer to discrete samples. For instance, (1.0, 1.0)
		// would refer to samples at indices (1, 1). Any decimal terms would
		// require interpolation
		math::Complex sampleDiscrete(math::real_d ku, math::real_d kv) const;

		void fillByteBuffer(ImageByteBuffer *target, Margins *margins = nullptr) const;

		void roll(ComplexMap2D *target) const;

		void fft(ComplexMap2D *target) const;
		void inverseFft(ComplexMap2D *target) const;

		void cft(ComplexMap2D *target, math::real_d physicalWidth, math::real_d physicalHeight) const;

		void magnitude();
		void multiply(const math::Complex &s);

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		void resizeSafe(ComplexMap2D *target, Margins *margins) const;
		void resize(int width, int height, ComplexMap2D *target, Margins *margins = nullptr) const;

		// Binary operations
		void multiply(const ComplexMap2D *b);
		void copy(const ComplexMap2D *b);
		void copy(const ScalarMap2D *b);
		void copy(const ImagePlane *b, int channel);
		void copy(const VectorMap2D *b, int channel);

	protected:
		math::Complex *m_data;

		int m_width;
		int m_height;
	};

} /* namespace manta */

#endif /* COMPLEX_MAP_2D_H */
