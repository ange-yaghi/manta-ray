#include "../include/render_pattern.h"

#include "../include/image_plane.h"
#include "../include/job_queue.h"

manta::RenderPattern::RenderPattern() {
    m_blockWidth = m_blockHeight = 0;
}

manta::RenderPattern::~RenderPattern() {
    /* void */
}

void manta::RenderPattern::generateJobs(const PatternParameters &parameters, JobQueue *target) {
    const int horizontalBlocks = (int)std::ceil(parameters.target->getWidth() / (float)m_blockWidth);
    const int verticalBlocks = (int)std::ceil(parameters.target->getHeight() / (float)m_blockHeight);

    std::vector<RenderBlock> blocks;
    generatePattern(horizontalBlocks, verticalBlocks, blocks);

    for (int i = 0; i < horizontalBlocks * verticalBlocks; ++i) {
        const RenderBlock &block = blocks[i];
        Job newJob;
        newJob.scene = parameters.scene;
        newJob.group = parameters.group;
        newJob.target = parameters.target;
        newJob.startX = block.x * m_blockWidth;
        newJob.startY = block.y * m_blockHeight;
        newJob.endX = (block.x + 1) * m_blockWidth - 1;
        newJob.endY = (block.y + 1) * m_blockHeight - 1;
        newJob.samples = 0;

        if (newJob.startX >= parameters.target->getWidth()) continue;
        if (newJob.startY >= parameters.target->getHeight()) continue;
        if (newJob.endX >= parameters.target->getWidth()) newJob.endX = parameters.target->getWidth() - 1;
        if (newJob.endY >= parameters.target->getHeight()) newJob.endY = parameters.target->getHeight() - 1;

        target->push(newJob);
    }
}

void manta::RenderPattern::_initialize() {
    /* void */
}

void manta::RenderPattern::_evaluate() {
    ObjectReferenceNode::_evaluate();

    piranha::native_int blockWidth, blockHeight;
    m_blockWidthInput->fullCompute((void *)&blockWidth);
    m_blockHeightInput->fullCompute((void *)&blockHeight);

    m_blockWidth = (int)blockWidth;
    m_blockHeight = (int)blockHeight;

    m_output.setReference(this);
}

void manta::RenderPattern::registerInputs() {
    registerInput(&m_blockWidthInput, "block_width");
    registerInput(&m_blockHeightInput, "block_height");
}
