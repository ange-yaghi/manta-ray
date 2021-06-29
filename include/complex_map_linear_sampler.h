#ifndef MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_H
#define MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_H

#include "complex_map_sampler.h"

#include "complex_map_2d.h"
#include "gaussian_filter.h"

#include <cmath>

namespace manta {

    class ComplexMapLinearSampler : public ComplexMapSampler {
    public:
        ComplexMapLinearSampler() {
            m_filter.setExtents({ 1.0f, 1.0f });
            m_filter.configure(4.0f);
        }

        virtual ~ComplexMapLinearSampler() {
            /* void */
        }

        virtual math::Complex sample(ComplexMap2D *map, math::real u, math::real v) {
            const int width = map->getWidth();
            const int height = map->getHeight();

            const math::real s = u * width;
            const math::real t = v * height;
            const int s0 = (int)std::floor(s), t0 = (int)std::floor(t);
            const math::real ds = s - s0, dt = t - t0;

            const math::Complex s1 = map->get(s0, t0) * (1 - ds) * (1 - dt);
            const math::Complex s2 = map->get(s0, t0 + 1) * (1 - ds) * dt;
            const math::Complex s3 = map->get(s0 + 1, t0) * ds * (1 - dt);
            const math::Complex s4 = map->get(s0 + 1, t0 + 1) * ds * dt;

            return s1 + s2 + s3 + s4;

            /*
            const int width = map->getWidth();
            const int height = map->getHeight();

            const math::real s = u * (width - 1);
            const math::real t = v * (height - 1);
            const int s0 = (int)std::floor(s), t0 = (int)std::floor(t);

            int samples = 0;
            int left = s0 - m_filter.getExtents().x * 2;
            int right = s0 + m_filter.getExtents().x * 2;
            int top = t0 - m_filter.getExtents().y * 2;
            int bottom = t0 + m_filter.getExtents().y * 2;

            math::Complex total;
            math::real weight = 0;
            for (int i = left; i <= right; ++i) {
                for (int j = top; j <= bottom; ++j) {
                    if (i < 0 || i >= width) continue;
                    if (j < 0 || j >= height) continue;

                    const math::Vector2 p = {
                        (i / ((math::real)width - 1) - u) * width,
                        (j / ((math::real)height - 1) - v) * height
                    };

                    if (abs(p.x) >= m_filter.getExtents().x * 2) continue;
                    if (abs(p.y) >= m_filter.getExtents().y * 2) continue;

                    const math::real w = math::getScalar(m_filter.evaluate(p));

                    total = total + map->get(i, j) * w;
                    weight += w;
                    ++samples;
                }
            }

            if (samples == 0) return math::Complex();
            else return total * (1 / weight);*/
        }

        GaussianFilter m_filter;
        
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_LINEAR_SAMPLER_H */
