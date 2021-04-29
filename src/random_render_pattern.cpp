#include "../include/random_render_pattern.h"

#include <random>

manta::RandomRenderPattern::RandomRenderPattern() {
    /* void */
}

manta::RandomRenderPattern::~RandomRenderPattern() {
    /* void */
}

void manta::RandomRenderPattern::generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks) {
    std::vector<int> d((size_t)verticalBlocks * horizontalBlocks);
    for (int i = 0; i < verticalBlocks * horizontalBlocks; ++i) d[i] = i;

    std::default_random_engine generator;

    for (int i = 0; i < horizontalBlocks * verticalBlocks; ++i) {
        std::uniform_int_distribution<int> distribution(0, horizontalBlocks * verticalBlocks - i - 1);
        const int d_i = distribution(generator);
        const int x = d[d_i];
        std::swap(d[d_i], d[(size_t)horizontalBlocks * verticalBlocks - i - 1]);

        blocks.push_back({ x % horizontalBlocks, x / horizontalBlocks });
    }
}
