#ifndef MANTARAY_VECTOR_CONVERSIONS_H
#define MANTARAY_VECTOR_CONVERSIONS_H

#include <piranha.h>

#include "vector_node_output.h"

namespace manta {

    class FloatToVectorConversionOutput : public VectorNodeOutput {
    public:
        FloatToVectorConversionOutput();
        ~FloatToVectorConversionOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        virtual void registerInputs();

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };
    typedef piranha::PipeNode<FloatToVectorConversionOutput> FloatToVectorConversionNode;

    class IntToVectorConversionOutput : public VectorNodeOutput {
    public:
        IntToVectorConversionOutput();
        ~IntToVectorConversionOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        virtual void registerInputs();

        piranha::pNodeInput *getInputConnection() { return &m_input; }

    protected:
        piranha::pNodeInput m_input;
    };
    typedef piranha::PipeNode<IntToVectorConversionOutput> IntToVectorConversionNode;

} /* namespace manta */

#endif /* MANTARAY_VECTOR_CONVERSIONS_H */
