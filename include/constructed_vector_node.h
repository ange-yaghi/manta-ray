#ifndef MANTARAY_CONSTRUCTED_VECTOR_NODE_H
#define MANTARAY_CONSTRUCTED_VECTOR_NODE_H

#include <piranha.h>

#include "constructed_vector_node_output.h"

namespace manta {

    class ConstructedVectorNode : public piranha::Node {
    public:
        ConstructedVectorNode();
        virtual ~ConstructedVectorNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        ConstructedVectorNodeOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_VECTOR_NODE_H */
