#ifndef MANTARAY_STEP_NODE_H
#define MANTARAY_STEP_NODE_H

#include "node.h"

#include "step_node_output.h"

namespace manta {

    class StepNode : public Node {
    public:
        StepNode();
        virtual ~StepNode();

        StepNodeOutput *getMainOutput() { return &m_output; }

    protected:
        StepNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_STEP_NODE_H */
