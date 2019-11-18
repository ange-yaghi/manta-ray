#ifndef MANTARAY_STRATIFIED_SAMPLER_H
#define MANTARAY_STRATIFIED_SAMPLER_H

#include "pixel_based_sampler.h"

#include <vector>

namespace manta {

    class StratifiedSampler : public PixelBasedSampler {
    public:
        StratifiedSampler();
        ~StratifiedSampler();

        virtual void startPixelSession();

        void setLatticeWidth(int width) { m_latticeWidth = width; }
        int getLatticeWidth() const { return m_latticeWidth; }

        void setLatticeHeight(int height) { m_latticeHeight = height; }
        int getLatticeHeight() const { return m_latticeHeight; }

        void setJitter(bool jitter) { m_jitter = jitter; }
        bool getJitter() const { return m_jitter; }

        virtual Sampler *clone() const;

    protected:
        void stratifiedSample1d(std::vector<math::real> &samples, int sampleCount, bool jitter);
        void stratifiedSample2d(std::vector<math::Vector2> &samples, int latticeWidth, int latticeHeight, bool jitter);

    protected:
        virtual void _evaluate();
        virtual void registerInputs();
       
        piranha::pNodeInput m_latticeWidthInput;
        piranha::pNodeInput m_latticeHeightInput;
        piranha::pNodeInput m_jitterInput;

        int m_latticeWidth;
        int m_latticeHeight;

        bool m_jitter;

    protected:
        template <typename T>
        void shuffle(std::vector<T> &v, int count, int dimensionCount) {
            for (int i = 0; i < count; i++) {
                int other = i + uniformRandomInt(count - i);
                for (int j = 0; j < dimensionCount; j++) {
                    std::swap(v[dimensionCount  * i + j],
                        v[dimensionCount * other + j]);
                }
            }
        }
    };

} /* namespace manta */

#endif /* MANTARAY_STRATIFIED_SAMPLER_H */
