#ifndef MANTARAY_SRGB_NODE_H
#define MANTARAY_SRGB_NODE_H

#include <piranha.h>

#include "srgb_node_output.h"

namespace manta {

    class SrgbNode : public piranha::Node {
    public:
        SrgbNode();
        virtual ~SrgbNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

        const SrgbNodeOutput *getMainOutput() const { return &m_output; }

    protected:
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

    protected:
        SrgbNodeOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_SRGB_NODE_H */
