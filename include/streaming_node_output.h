#ifndef MANTARAY_STREAMING_NODE_OUTPUT_H
#define MANTARAY_STREAMING_NODE_OUTPUT_H

#include <piranha.h>

namespace manta {

    struct IntersectionPoint;

    class StreamingNodeOutput : public piranha::NodeOutput {
    public:
        static const int MAX_DIMENSIONS = 4;

    public:
        StreamingNodeOutput(const piranha::ChannelType *channelType);
        virtual ~StreamingNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const { /* void */ }
        virtual void discreteSample2d(int x, int y, void *target) const { /* void */ }
        virtual void fullCompute(void *target) const { /* void */ }
        virtual void getDataReference(const void **target) const { *target = nullptr; }

        int getSize(int dim) const { return m_dimensions[dim]; }
        int getDimensions() const { return m_dimensionCount; }
        int getLargestDimensionSize() const;

        void evaluateDimensions();
        bool areDimensionsEvaluated() const { return m_dimensionsEvaluated; }

    protected:
        void setDimensionSize(int dim, int size) { m_dimensions[dim] = size; }
        void setDimensions(int dimensionCount) { m_dimensionCount = dimensionCount; }

        virtual void _evaluate();
        virtual void _evaluateDimensions();

    protected:
        int m_dimensions[MAX_DIMENSIONS];
        int m_dimensionCount;

        bool m_dimensionsEvaluated;
    };

} /* namespace mantaray */

#endif /* MANTARAY_STREAMING_NODE_OUTPUT_H */
