#ifndef MIPMAP_H
#define MIPMAP_H

#include <standard_allocator.h>

#include <vector>
#include <assert.h>

// For Intellisense only
#include <scalar_map_2d.h>

namespace manta {

	template <typename Map, typename ValueType>
	class Mipmap {
	public:
		Mipmap() {
			m_maps = nullptr;

			m_maxHeight = 0;
			m_maxWidth = 0;

			m_levels = 0;
		}

		~Mipmap() {
			assert(m_maps == nullptr);
		}

		static int calculateLevels(int size) {
			int currentSize = size;
			int levels = 0;
			while (currentSize > 0) {
				currentSize >>= 1;
				levels++;
			}

			return levels;
		}

		void initialize(const Map *map) {
			m_maxWidth = map->getWidth();
			m_maxHeight = map->getHeight();

			int currentSize = (m_maxHeight < m_maxWidth) ? m_maxHeight : m_maxWidth;
			m_levels = calculateLevels(currentSize);
			m_maps = StandardAllocator::Global()->allocate<Map>(m_levels, 16);
			m_maps[0].copy(map);

			for (int i = 1; i < m_levels; i++) {
				m_maps[i - 1].boxDownsample(&m_maps[i]);
			}
		}

		ValueType triangle(int level, math::real u, math::real v) const {
			if (level < 0) level = 0;
			else if (level >= m_levels) level = m_levels - 1;

			math::real s = u * m_maps[level].getWidth() - (math::real)0.5;
			math::real t = v * m_maps[level].getHeight() - (math::real)0.5;
			int s0 = (int)std::floor(s), t0 = (int)std::floor(t);
			math::real ds = s - s0, dt = t - t0;
			return discreteSample(level, s0, t0) * (1 - ds) * (1 - dt) +
				discreteSample(level, s0, t0 + 1) * (1 - ds) * dt +
				discreteSample(level, s0 + 1, t0) * ds * (1 - dt) +
				discreteSample(level, s0 + 1, t0 + 1) * ds * dt;
		}

		ValueType discreteSample(int level, int i, int j) const {
			return m_maps[level].get(i, j);
		}

		ValueType sample(math::real u, math::real v, math::real w) const {
			math::real level = m_levels - 1 + std::log2(std::fmax(w, (math::real)1E-8));

			if (level < 0) {
				return triangle(0, u, v);
			}
			else if (level >= m_levels - 1) {
				return discreteSample(m_levels - 1, 0, 0);
			}
			else {
				int iLevel = (int)std::floor(level);
				math::real delta = level - iLevel;
				return triangle(iLevel, u, v) * (1 - delta) + triangle(iLevel + 1, u, v) * delta;
			}
		}

		void destroy() {
			for (int i = 0; i < m_levels; i++) {
				m_maps[i].destroy();
			}

			StandardAllocator::Global()->aligned_free(m_maps, m_levels);

			m_maps = nullptr;
			m_levels = 0;
		}

		int getLevels() const {
			return m_levels;
		}

	protected:
		Map *m_maps;

		int m_maxHeight;
		int m_maxWidth;

		int m_levels;
	};

} /* namespace manta */

#endif /* MIPMAP_H */
