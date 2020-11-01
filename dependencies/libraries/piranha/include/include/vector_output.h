#ifndef PIRANHA_VECTOR_OUTPUT_H
#define PIRANHA_VECTOR_OUTPUT_H

#include "fundamental_output.h"

#include "memory_tracker.h"
#include "vector_split_node.h"

namespace piranha {

    template <>
    class FundamentalOutput<piranha::vector> : public NodeOutput {
    public:
        FundamentalOutput() : NodeOutput(&FundamentalType::VectorType) { /* void */ };
        ~FundamentalOutput() { /* void */ }

    protected:
        virtual Node *newInterface(NodeAllocator *nodeAllocator) {
            VectorSplitNode *vectorInterface = nodeAllocator->allocate<VectorSplitNode>();
            vectorInterface->initialize();
            vectorInterface->connectInput(this, "__in", nullptr);

            return vectorInterface;
        }
    };

    typedef FundamentalOutput<piranha::native_vector> VectorValueOutput;

} /* namespace piranha */

#endif /* PIRANHA_VECTOR_OUTPUT_H */
