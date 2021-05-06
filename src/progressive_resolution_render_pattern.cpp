#include "../include/progressive_resolution_render_pattern.h"

#include <random>
#include <queue>

manta::ProgressiveResolutionRenderPattern::ProgressiveResolutionRenderPattern() {
    m_size = 16;
    m_sizeInput = nullptr;
}

manta::ProgressiveResolutionRenderPattern::~ProgressiveResolutionRenderPattern() {
    /* void */
}

void manta::ProgressiveResolutionRenderPattern::generateLayer(
    int horizontalBlocksFull,
    int verticalBlocksFull,
    int s,
    int o_x,
    int o_y,
    std::vector<RenderBlock> &blocks)
{
    const int horizontalBlocks = std::ceil((double)horizontalBlocksFull / s);
    const int verticalBlocks = std::ceil((double)verticalBlocksFull / s);

    std::vector<int> d((size_t)verticalBlocks * horizontalBlocks);
    for (int i = 0; i < verticalBlocks * horizontalBlocks; ++i) d[i] = i;

    std::default_random_engine generator;

    for (int i = 0; i < horizontalBlocks * verticalBlocks; ++i) {
        std::uniform_int_distribution<int> distribution(0, horizontalBlocks * verticalBlocks - i - 1);
        const int d_i = distribution(generator);
        const int x = d[d_i];
        std::swap(d[d_i], d[(size_t)horizontalBlocks * verticalBlocks - i - 1]);

        const int p_x = (x % horizontalBlocks) * s + o_x;
        const int p_y = (x / horizontalBlocks) * s + o_y;

        if (p_x >= 0 && p_x < horizontalBlocksFull && p_y >= 0 && p_y < verticalBlocksFull) {
            blocks.push_back({ p_x, p_y });
        }
    }
}

void manta::ProgressiveResolutionRenderPattern::generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks) {    
    struct StackData {
        int s;
        int o_x;
        int o_y;
        bool generate = true;
    };

    std::queue<StackData> q;
    q.push({ m_size, 0, 0});

    blocks.clear();

    std::default_random_engine generator;
    while (!q.empty()) {
        const StackData data = q.front(); q.pop();

        if (data.s > 1) {
            StackData newBranches[] = {
                { data.s / 2, data.o_x, data.o_y, false },
                { data.s / 2, data.o_x + data.s / 2, data.o_y },
                { data.s / 2, data.o_x, data.o_y + data.s / 2 },
                { data.s / 2, data.o_x + data.s / 2, data.o_y + data.s / 2 }
            };

            int rand[] = { 0, 1, 2, 3 };
            for (int i = 0; i < 4; ++i) {
                std::uniform_int_distribution<int> distribution(0, 4 - i - 1);
                const int d_i = distribution(generator);
                std::swap(rand[d_i], rand[(size_t)4 - i - 1]);
            }

            for (int i = 0; i < 4; ++i) {
                q.push(newBranches[rand[i]]);
            }
        }
        
        if (data.generate) {
            generateLayer(horizontalBlocks, verticalBlocks, m_size, data.o_x, data.o_y, blocks);
        }
    }

    assert(blocks.size() == ((size_t)horizontalBlocks * verticalBlocks));
}

void manta::ProgressiveResolutionRenderPattern::_evaluate() {
    RenderPattern::_evaluate();

    piranha::native_int size;
    m_sizeInput->fullCompute((void *)&size);

    m_size = 0x1 << size;
}

void manta::ProgressiveResolutionRenderPattern::registerInputs() {
    RenderPattern::registerInputs();

    registerInput(&m_sizeInput, "size");
}
