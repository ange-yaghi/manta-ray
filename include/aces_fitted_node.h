#ifndef MANTARAY_ACES_FITTED_NODE_H
#define MANTARAY_ACES_FITTED_NODE_H

#include "node.h"

#include "aces_fitted_node_output.h"

namespace manta {

    class ACESFittedNode : public Node {
    public:
        ACESFittedNode();
        virtual ~ACESFittedNode();

        ACESFittedNodeOutput *getMainOutput() { return &m_output; }

    protected:
        ACESFittedNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_ACES_FITTED_NODE_H */
