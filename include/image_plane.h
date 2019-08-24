#ifndef MANTARAY_IMAGE_PLANE_H
#define MANTARAY_IMAGE_PLANE_H

#include "image_sample.h"
#include "job_queue.h"
#include "manta_math.h"

#include <condition_variable>
#include <mutex>

namespace manta {

    class ImagePlane {
    protected:
        static constexpr int DEFAULT_QUEUE_CAPACITY = 2 << 16;

    public:
        ImagePlane();
        ~ImagePlane();

        void initialize(int width, int height, int queueCapacity = DEFAULT_QUEUE_CAPACITY);
        void destroy();

        bool checkPixel(int x, int y) const;
        inline void set(const math::Vector &v, int x, int y);
        inline math::Vector sample(int x, int y) const;

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        bool isInitialized() const { return m_buffer != nullptr; }

        void copyFrom(const ImagePlane *source);
        void createEmptyFrom(const ImagePlane *source);
        void clear(const math::Vector &v = math::constants::Zero);

        const math::Vector *getBuffer() const { return m_buffer; }

        void reset();
        void addSamples(ImageSample *samples, int sampleCount);
        static void processLoop(ImagePlane *target);
        bool processAllSamples();
        void terminate();

        void normalize();

    protected:
        int m_width;
        int m_height;
        math::Vector *m_buffer;
        math::real *m_sampleWeightSums;

    protected:
        ImageSample *m_sampleQueue;
        int m_queueLength;
        int m_queueOffset;
        int m_queueCapacity;
        bool m_done;

        std::mutex m_queueLock;
        std::mutex m_dataConditionLock;
        std::condition_variable m_dataCondition;

        std::mutex m_emptyConditionLock;
        std::condition_variable m_emptyCondition;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_PLANE_H */
