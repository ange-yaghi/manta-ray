#ifndef MANTARAY_MULTIPLY_NODE_H
#define MANTARAY_MULTIPLY_NODE_H

#include <piranha.h>

#include "vector_node_output.h"
#include "multiply_node_output.h"
#include "manta_math.h"

namespace manta {

    class MultiplyNode : public piranha::Node {
    public:
        MultiplyNode();
        virtual ~MultiplyNode();

        MultiplyNodeOutput *getMainOutput() { return &m_output; }

    protected:
        MultiplyNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_MULTIPLY_NODE_H */
