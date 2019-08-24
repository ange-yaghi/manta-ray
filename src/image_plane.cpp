#include "../include/image_plane.h"

#include "../include/standard_allocator.h"

#include <assert.h>
#include <iostream>

manta::ImagePlane::ImagePlane() {
    m_width = 0;
    m_height = 0;
    m_buffer = nullptr;
    m_sampleWeightSums = nullptr;

    m_queueLength = 0;
    m_queueOffset = 0;
    m_queueCapacity = DEFAULT_QUEUE_CAPACITY;
    m_sampleQueue = nullptr;

    m_done = false;
}

manta::ImagePlane::~ImagePlane() {
    assert(m_buffer == nullptr);
    assert(m_sampleQueue == nullptr);
    assert(m_sampleWeightSums == nullptr);
}

void manta::ImagePlane::initialize(int width, int height, int queueCapacity) {
    assert(width != 0);
    assert(height != 0);

    m_width = width;
    m_height = height;

    m_queueCapacity = queueCapacity;

    int pixelCount = width * height;

    m_buffer = (math::Vector *)_aligned_malloc(sizeof(math::Vector) * pixelCount, 16);
    m_sampleWeightSums = StandardAllocator::Global()->allocate <math::real>(pixelCount);
    m_sampleQueue = StandardAllocator::Global()->allocate<ImageSample>(queueCapacity, 16);

    for (int i = 0; i < pixelCount; i++) {
        m_buffer[i] = math::constants::Zero;
        m_sampleWeightSums[i] = (math::real)0.0;
    }

    assert(m_buffer != nullptr);
}

void manta::ImagePlane::destroy() {
    assert(m_buffer != nullptr);

    _aligned_free(m_buffer);
    StandardAllocator::Global()->aligned_free(m_sampleQueue);
    StandardAllocator::Global()->free(m_sampleWeightSums);

    // Reset member variables
    m_buffer = nullptr;
    m_sampleQueue = nullptr;
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

void manta::ImagePlane::reset() {
    m_done = false;
}

void manta::ImagePlane::addSamples(ImageSample *samples, int sampleCount) {
    std::unique_lock<std::mutex> lock(m_queueLock);

    // Check if the queue is full and if so, wait until it has enough space
    if (m_queueLength + sampleCount > m_queueCapacity) 
        m_emptyCondition.wait(lock);

    for (int i = 0; i < sampleCount; i++) {
        int index = (i + m_queueOffset + m_queueLength) % m_queueCapacity;
        m_sampleQueue[index] = samples[i];
    }

    m_queueLength += sampleCount;

    // Notify the processing thread that there is now data
    m_dataCondition.notify_one();
}

void manta::ImagePlane::processLoop(ImagePlane *target) {
    while (target->processAllSamples());
}

bool manta::ImagePlane::processAllSamples() {
    std::unique_lock<std::mutex> lock(m_queueLock);
    if (m_done) return false;

    while (m_queueLength == 0) {
        m_dataCondition.wait(lock);
        if (m_done && m_queueLength == 0) return false;
    }

    int queueStart = 0;
    int queueLength = 0;

    queueStart = m_queueOffset;
    queueLength = m_queueLength;
    lock.unlock();

    for (int i = 0; i < queueLength; i++) {
        int index = (i + queueStart) % m_queueCapacity;

        // Box filter implementation for now
        const ImageSample &sample = m_sampleQueue[index];
        int x = (int)(sample.imagePlaneLocation.x + (math::real)0.5);
        int y = (int)(sample.imagePlaneLocation.y + (math::real)0.5);

        bool inBounds = checkPixel(x, y);
        if (!inBounds) continue;

        math::Vector &value = m_buffer[y * m_width + x];
        math::real &weightSum = m_sampleWeightSums[y * m_width + x];

        math::real weight = (math::real)1.0;

        value = math::add(value, math::mul(sample.intensity, math::loadScalar(weight)));
        value = math::add(value, sample.intensity);
        weightSum += weight;
    }

    lock.lock();
    m_queueLength -= queueLength;
    m_queueOffset = (m_queueOffset + queueLength) % m_queueCapacity;

    m_emptyCondition.notify_all();

    return !m_done;
}

void manta::ImagePlane::terminate() {
    std::unique_lock<std::mutex> lock(m_queueLock);

    m_done = true;
    m_dataCondition.notify_one();
}

void manta::ImagePlane::normalize() {
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            math::Vector &value = m_buffer[y * m_width + x];
            math::real weightSum = m_sampleWeightSums[y * m_width + x];

            value = math::div(value, math::loadScalar(weightSum));
        }
    }
}
