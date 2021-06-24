#ifndef MANTARAY_COMPLEX_SPLIT_NODE_H
#define MANTARAY_COMPLEX_SPLIT_NODE_H

#include <piranha.h>

#include "complex_split_node_output.h"

namespace manta {

    class ComplexSplitNode : public piranha::Node {
    public:
        ComplexSplitNode();
        virtual ~ComplexSplitNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_input;

        ComplexSplitNodeOutput m_r;
        ComplexSplitNodeOutput m_i;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_SPLIT_NODE_H */
