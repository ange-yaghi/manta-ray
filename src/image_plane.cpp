#include "../include/image_plane.h"

#include "../include/standard_allocator.h"
#include "../include/gaussian_filter.h"
#include "../include/triangle_filter.h"
#include "../include/box_filter.h"

#include <assert.h>
#include <iostream>

manta::ImagePlane::ImagePlane() {
    m_width = 0;
    m_height = 0;
    m_buffer = nullptr;
    m_sampleWeightSums = nullptr;
    m_filter = nullptr;
}

manta::ImagePlane::~ImagePlane() {
    assert(m_buffer == nullptr);
    assert(m_sampleWeightSums == nullptr);
}

void manta::ImagePlane::initialize(int width, int height) {
    assert(width != 0);
    assert(height != 0);

    m_width = width;
    m_height = height;

    int pixelCount = width * height;

    m_buffer = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * pixelCount, 16);
    m_sampleWeightSums = StandardAllocator::Global()->allocate <math::real>(pixelCount);

    for (int i = 0; i < pixelCount; i++) {
        m_buffer[i] = math::constants::Zero;
        m_sampleWeightSums[i] = (math::real)0.0;
    }

    assert(m_buffer != nullptr);
}

void manta::ImagePlane::destroy() {
    assert(m_buffer != nullptr);
    assert(m_sampleWeightSums != nullptr);

    _aligned_free(m_buffer);
    StandardAllocator::Global()->free(m_sampleWeightSums);

    // Reset member variables
    m_buffer = nullptr;
    m_sampleWeightSums = nullptr;
    m_width = 0;
    m_height = 0;    
}

bool manta::ImagePlane::checkPixel(int x, int y) const {
    return (x < m_width && x >= 0) && (y < m_height && y >= 0);
}

inline void manta::ImagePlane::set(const math::Vector &v, int x, int y) {
    assert(x < m_width && x >= 0);
    assert(y < m_height && y >= 0);

    m_buffer[y * m_width + x] = v;
}

manta::math::Vector manta::ImagePlane::sample(int x, int y) const {
    assert(x < m_width && x >= 0);
    assert(y < m_height && y >= 0);

    return m_buffer[y * m_width + x];
}

void manta::ImagePlane::copyFrom(const ImagePlane *source) {
    initialize(m_width, m_height);
    for (int x = 0; x < (m_width); x++) {
        for (int y = 0; y < (m_height); y++) {
            set(sample(x, y), x, y);
        }
    }
}

void manta::ImagePlane::createEmptyFrom(const ImagePlane *source) {
    initialize(source->m_width, source->m_height);
}

void manta::ImagePlane::clear(const math::Vector &v) {
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            set(v, x, y);
        }
    }
}

#define FAST_ABS(x) (((x) > 0) ? (x) : -(x))

void manta::ImagePlane::processSamples(ImageSample *samples, int sampleCount, StackAllocator *stack) {
    struct Block {
        math::Vector value;
        math::real weight;
        int x, y;
    };

    Block *blocks = (Block *)stack->allocate(sizeof(Block), 16);
    Block *currentBlock = blocks;
    int blockCount = 0;

    for (int i = 0; i < sampleCount; i++) {
        const ImageSample &sample = samples[i];
        math::Vector2 extents = m_filter->getExtents();
        int left = (int)(floor(sample.imagePlaneLocation.x - extents.x));
        int right = (int)(ceil(sample.imagePlaneLocation.x + extents.x) + (math::real)0.5);
        int top = (int)(floor(sample.imagePlaneLocation.y - extents.y));
        int bottom = (int)(ceil(sample.imagePlaneLocation.y + extents.y) + (math::real)0.5); 

        for (int x = left; x <= right; x++) {
            for (int y = top; y <= bottom; y++) {
                bool inBounds = checkPixel(x, y);
                if (!inBounds) continue;

                math::Vector2 p(
                    sample.imagePlaneLocation.x - (math::real)x, 
                    sample.imagePlaneLocation.y - (math::real)y
                );

                if (FAST_ABS(p.x) > extents.x || FAST_ABS(p.y) > extents.y) continue;

                math::Vector weight = m_filter->evaluate(p);
                currentBlock->value = math::mul(weight, sample.intensity);
                currentBlock->weight = math::getScalar(weight);
                currentBlock->x = x;
                currentBlock->y = y;
                currentBlock++;
                blockCount++;
            }
        }
    }

    std::unique_lock<std::mutex> lock(m_lock);
    for (int i = 0; i < blockCount; i++) {
        const Block &block = blocks[i];
        
        math::Vector &value = m_buffer[block.y * m_width + block.x];
        math::real &weightSum = m_sampleWeightSums[block.y * m_width + block.x];

        value = math::add(value, block.value);
        weightSum += block.weight;
    }

    stack->free(blocks);
}

void manta::ImagePlane::normalize() {
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            math::Vector &value = m_buffer[y * m_width + x];
            math::real weightSum = m_sampleWeightSums[y * m_width + x];

            value = math::div(value, math::loadScalar(weightSum));

            constexpr math::Vector DEBUG_RED = { (math::real)1.0, (math::real)0.0, (math::real)0.0 };
            constexpr math::Vector DEBUG_BLUE = { (math::real)0.0, (math::real)0.0, (math::real)1.0 };
            constexpr math::Vector DEBUG_GREEN = { (math::real)0.0, (math::real)1.0, (math::real)0.0 };

            if (std::isnan(math::getX(value)) || std::isnan(math::getY(value)) || std::isnan(math::getZ(value))) {
                value = DEBUG_RED;
            }
            else if (std::isinf(math::getX(value)) || std::isinf(math::getY(value)) || std::isinf(math::getZ(value))) {
                value = DEBUG_GREEN;
            }
            else if (math::getX(value) < 0 || math::getY(value) < 0 || math::getZ(value) < 0) {
                value = DEBUG_BLUE;
            }
        }
    }
}
