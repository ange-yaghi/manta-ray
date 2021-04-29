#ifndef MANTARAY_SPIRAL_RENDER_PATTERN_H
#define MANTARAY_SPIRAL_RENDER_PATTERN_H

#include "render_pattern.h"

#include "manta_math.h"

namespace manta {

    class JobQueue;

    class SpiralRenderPattern : public RenderPattern {
    public:
        SpiralRenderPattern();
        virtual ~SpiralRenderPattern();

    protected:
        virtual void generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks);
    };

} /* namespace manta */

#endif /* MANTARAY_SPIRAL_RENDER_PATTERN_H */
