#ifndef MANTARAY_RANDOM_RENDER_PATTERN_H
#define MANTARAY_RANDOM_RENDER_PATTERN_H

#include "render_pattern.h"

#include "manta_math.h"

namespace manta {

    class RandomRenderPattern : public RenderPattern {
    public:
        RandomRenderPattern();
        virtual ~RandomRenderPattern();

    protected:
        virtual void generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks);
    };

} /* namespace manta */

#endif /* MANTARAY_RANDOM_RENDER_PATTERN_H */
