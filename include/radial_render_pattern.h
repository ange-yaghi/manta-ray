#ifndef MANTARAY_RADIAL_RENDER_PATTERN_H
#define MANTARAY_RADIAL_RENDER_PATTERN_H

#include "render_pattern.h"

#include "manta_math.h"

namespace manta {

    class RadialRenderPattern : public RenderPattern {
    public:
        RadialRenderPattern();
        virtual ~RadialRenderPattern();

    protected:
        virtual void generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks);
    };

} /* namespace manta */

#endif /* MANTARAY_RADIAL_RENDER_PATTERN_H */
