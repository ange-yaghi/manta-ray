#ifndef MANTARAY_PROGRESSIVE_RESOLUTION_RENDER_PATTERN_H
#define MANTARAY_PROGRESSIVE_RESOLUTION_RENDER_PATTERN_H

#include "render_pattern.h"

#include "manta_math.h"

namespace manta {

    class ProgressiveResolutionRenderPattern : public RenderPattern {
    public:
        ProgressiveResolutionRenderPattern();
        virtual ~ProgressiveResolutionRenderPattern();

    protected:
        virtual void generateLayer(int horizontalBlocks, int verticalBlocks, int s, int o_x, int o_y, std::vector<RenderBlock> &blocks);
        virtual void generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks);

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_sizeInput;
        int m_size;
    };

} /* namespace manta */

#endif /* MANTARAY_PROGRESSIVE_RESOLUTION_RENDER_PATTERN_H */
