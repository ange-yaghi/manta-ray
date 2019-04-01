#ifndef SCALAR_MAP_2D_H
#define SCALAR_MAP_2D_H

#include <manta_math.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class VectorMap2D;

	class ScalarMap2D {
	public:
		ScalarMap2D();
		~ScalarMap2D();

		void initialize(int width, int height, math::real value = (math::real)0.0);
		void destroy();

		math::real sample(math::real u, math::real v) const;
		math::real get(int u, int v) const;
		void set(math::real value, int u, int v);

		void fillByteBuffer(ImageByteBuffer *target) const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		// Binary operations
		void multiply(const ScalarMap2D *b);
		void copy(const ScalarMap2D *b);
		void copy(const VectorMap2D *b);

	protected:
		math::real *m_data;

		int m_width;
		int m_height;
	};

} /* namespace manta */

#endif /* SCALAR_MAP_2D_H */
