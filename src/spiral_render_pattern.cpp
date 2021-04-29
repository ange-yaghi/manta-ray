#include "../include/spiral_render_pattern.h"

manta::SpiralRenderPattern::SpiralRenderPattern() {
    /* void */
}

manta::SpiralRenderPattern::~SpiralRenderPattern() {
    /* void */
}

void manta::SpiralRenderPattern::generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks) {
    const int start_x = horizontalBlocks / 2;
    const int start_y = verticalBlocks / 2;
    int dx = 1;
    int dy = 0;

    int x = start_x;
    int y = start_y;

    int min_x = start_x;
    int max_x = start_y;
    int min_y = start_x;
    int max_y = start_y;

    if (x >= horizontalBlocks || y >= verticalBlocks) return;

    while (blocks.size() < ((size_t)horizontalBlocks * verticalBlocks)) {
        if (x < horizontalBlocks && x >= 0 && y < verticalBlocks && y >= 0) {
            blocks.push_back({ x, y });
        }

        x += dx;
        y += dy;

        if (x > max_x) {
            dx = 0;
            dy = 1;
        }
        else if (x < min_x) {
            dx = 0;
            dy = -1;
        }
        else if (y > max_y) {
            dx = -1;
            dy = 0;
        }
        else if (y < min_y) {
            dx = 1;
            dy = 0;
        }

        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
    }
}
