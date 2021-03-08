#ifndef MANTARAY_IMAGE_PLANE_H
#define MANTARAY_IMAGE_PLANE_H

#include "object_reference_node.h"

#include "image_sample.h"
#include "job_queue.h"
#include "manta_math.h"
#include "stack_allocator.h"
#include "session.h"

#include <condition_variable>
#include <mutex>
#include <fstream>

namespace manta {

    class Filter;

    class ImagePlane : public ObjectReferenceNode<ImagePlane> {
    public:
        ImagePlane();
        ~ImagePlane();

        void initialize(int width, int height);
        void destroy();

        bool checkPixel(int x, int y) const;
        inline void set(const math::Vector &v, int x, int y);
        inline math::Vector sample(int x, int y) const;

        void setFilter(Filter *filter) { m_filter = filter; }
        Filter *getFilter() const { return m_filter; }

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }

        bool isInitialized() const { return m_buffer != nullptr; }

        void copyFrom(const ImagePlane *source);
        void createEmptyFrom(const ImagePlane *source);
        void clear(const math::Vector &v = math::constants::Zero);

        const math::Vector *getBuffer() const { return m_buffer; }

        void processSamples(ImageSample *samples, int sampleCount, StackAllocator *stack);

        void normalize();

    protected:
        virtual void _evaluate();
        virtual void _initialize();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::pNodeInput m_filterInput;
        piranha::pNodeInput m_imagePlanePreviewInput;

        ImagePlanePreview *m_previewTarget;

    protected:
        int m_width;
        int m_height;
        math::Vector *m_buffer;
        math::real *m_sampleWeightSums;
        Filter *m_filter;

    protected:
        std::mutex m_lock;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_PLANE_H */
