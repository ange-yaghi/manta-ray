#ifndef MANTARAY_BUMP_NODE_H
#define MANTARAY_BUMP_NODE_H

#include <piranha.h>

#include "bump_node_output.h"

namespace manta {

    class BumpNode : public piranha::Node {
    public:
        BumpNode();
        virtual ~BumpNode();

        BumpNodeOutput *getMainOutput() { return &m_output; }

    protected:
        BumpNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_BUMP_NODE_H */
