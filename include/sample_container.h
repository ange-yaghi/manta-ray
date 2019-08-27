#ifndef MANTARAY_SAMPLE_CONTAINER_H
#define MANTARAY_SAMPLE_CONTAINER_H

namespace manta {

    // Forward declarations
    class ImageSample;
    class StackAllocator;

    class SampleContainer {
    public:
        SampleContainer();
        virtual ~SampleContainer();
        
        ImageSample *getSample() const { return m_samples; }
        int getRayCount() const { return m_rayCount; }

        void initializeSamples(int count);
        void destroySamples();

        void setStackAllocator(StackAllocator *allocator) { m_stackAllocator = allocator; }
        StackAllocator *getStackAllocator() const { return m_stackAllocator; }

    private:
        int m_rayCount;

        ImageSample *m_samples;

    private:
        StackAllocator *m_stackAllocator;
    };

} /* namespace manta */

#endif /* MANTARAY_SAMPLE_CONTAINER_H */
