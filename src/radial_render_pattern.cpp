#include "../include/radial_render_pattern.h"

#include <algorithm>

manta::RadialRenderPattern::RadialRenderPattern() {
    /* void */
}

manta::RadialRenderPattern::~RadialRenderPattern() {
    /* void */
}

void manta::RadialRenderPattern::generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks) {
    const int origin_x = horizontalBlocks / 2;
    const int origin_y = verticalBlocks / 2;

    for (int i = 0; i < horizontalBlocks; ++i) {
        for (int j = 0; j < verticalBlocks; ++j) {
            blocks.push_back({ i - origin_x, j - origin_y });
        }
    }

    struct {
        bool operator()(const RenderBlock &a, const RenderBlock &b) const {
            return (a.x * a.x + a.y * a.y) < (b.x * b.x + b.y * b.y);
        }
    } DistanceCompare;

    std::sort(blocks.begin(), blocks.end(), DistanceCompare);

    for (int i = 0; i < horizontalBlocks; ++i) {
        for (int j = 0; j < verticalBlocks; ++j) {
            blocks[(size_t)i * verticalBlocks + j].x += origin_x;
            blocks[(size_t)i * verticalBlocks + j].y += origin_y;
        }
    }
}
