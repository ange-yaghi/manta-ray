#ifndef MANTARAY_PADDED_FRAME_OUTPUT_H
#define MANTARAY_PADDED_FRAME_OUTPUT_H

#include "vector_node_output.h"

#include "unary_node.h"

namespace manta {

    class PaddedFrameOutput : public piranha::NodeOutput {
    public:
        static int paddedResize(int originalSize) {
            int minResize = originalSize * 2;

            int size = 1;
            while (size < minResize) size *= 2;

            return size;
        }

    public:
        PaddedFrameOutput() : piranha::NodeOutput(piranha::NativeTypeLookup<piranha::native_int>()) {
            m_input = nullptr;
        }

        virtual ~PaddedFrameOutput() {
            /* void */
        }

        virtual void fullCompute(void *_target) const {
            piranha::native_int *target = reinterpret_cast<piranha::native_int *>(_target);

            VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
            int maxSize = input->getLargestDimensionSize();

            *target = paddedResize(maxSize);
        }

        piranha::pNodeInput *getConnection() { return &m_input; }

    protected:
        virtual void registerInputs() {
            registerInput(&m_input);
        }

    protected:
        piranha::pNodeInput m_input;
    };

    typedef SingleOutputNode<PaddedFrameOutput> PaddedFrameNode;

} /* namespace manta */

#endif /* MANTARAY_PADDED_FRAME_OUTPUT_H */
