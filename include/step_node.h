#ifndef MANTARAY_STEP_NODE_H
#define MANTARAY_STEP_NODE_H

#include <piranha.h>

#include "vector_node_output.h"
#include "step_node_output.h"
#include "manta_math.h"

namespace manta {

    class StepNode : public piranha::Node {
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
