#ifndef SCALAR_MAP_2D_H
#define SCALAR_MAP_2D_H

#include <manta_math.h>
#include <standard_allocator.h>
#include <image_byte_buffer.h>
#include <vector_map_2d.h>

#include <assert.h>

namespace manta {

	// Forward declarations
	class ImageByteBuffer;
	class VectorMap2D;

	template <typename s>
	class ScalarMap2D {
	public:
		ScalarMap2D() {
			m_data = nullptr;
		}

		~ScalarMap2D() {
			assert(m_data == nullptr);
		}

		void initialize(int width, int height, s value = (math::real)0.0) {
			m_width = width;
			m_height = height;

			m_data = StandardAllocator::Global()->allocate<math::real>(m_width * m_height, 16);

			for (int i = 0; i < m_width * m_height; i++) {
				m_data[i] = (math::real)0.0;
			}
		}

		void destroy() {
			StandardAllocator::Global()->aligned_free(m_data, m_width * m_height);

			m_data = nullptr;
			m_width = 0;
			m_height = 0;
		}

		s sample(math::real u, math::real v) const {
			// Very simple sampling for now
			int iu = (int)(u + (math::real)0.5);
			int iv = (int)(v + (math::real)0.5);

			if (iu < 0 || iu >= m_width) return (math::real)0.0;
			if (iv < 0 || iv >= m_width) return (math::real)0.0;

			return get(iu, iv);
		}

		s get(int u, int v) const {
			assert(m_data != nullptr);
			u %= m_width;
			v %= m_height;

			if (u < 0) u += m_width;
			if (v < 0) v += m_height;

			return m_data[v * m_width + u];
		}

		void set(math::real value, int u, int v) {
			assert(m_data != nullptr);
			m_data[v * m_width + u] = value;
		}

		void fillByteBuffer(ImageByteBuffer *target, bool correctGamma) const {
			target->initialize(m_data, m_width, m_height, correctGamma);
		}

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		// Binary operations
		void multiply(const ScalarMap2D *b) {
			for (int i = 0; i < m_width; i++) {
				for (int j = 0; j < m_height; j++) {
					set(get(i, j) * b->get(i, j), i, j);
				}
			}
		}

		void copy(const ScalarMap2D *b) {
			initialize(b->getWidth(), b->getHeight());

			for (int i = 0; i < m_width; i++) {
				for (int j = 0; j < m_height; j++) {
					set(b->get(i, j), i, j);
				}
			}
		}

		void copy(const VectorMap2D *b) {
			initialize(b->getWidth(), b->getHeight());

			for (int i = 0; i < m_width; i++) {
				for (int j = 0; j < m_height; j++) {
					set(math::getScalar(b->get(i, j)), i, j);
				}
			}
		}

		// Resampling operations
		void boxDownsample(ScalarMap2D *target) const {
			target->initialize(m_width / 2, m_height / 2);

			for (int i = 0; i < m_width; i += 2) {
				for (int j = 0; j < m_height; j += 2) {
					math::real sum = 
						get(i, j) + get(i + 1, j) + get(i, j + 1) + get(i + 1, j + 1);
					sum /= 4;

					target->set(sum, i / 2, j / 2);
				}
			}
		}

	protected:
		math::real *m_data;

		int m_width;
		int m_height;
	};

	typedef ScalarMap2D<math::real> RealMap2D;
	typedef ScalarMap2D<math::real_f> RealMap2D_f;
	typedef ScalarMap2D<math::real_d> RealMap2D_d;

} /* namespace manta */

#endif /* SCALAR_MAP_2D_H */
