#ifndef MANTARAY_UNARY_NODE_H
#define MANTARAY_UNARY_NODE_H

#include "node.h"

#include "unary_node_output.h"

namespace manta {

    template <UNARY_OPERATION op>
    class UnaryNode : public Node {
    public:
        UnaryNode() {
            /* void */
        }

        virtual ~UnaryNode() {
            /* void */
        }

    protected:
        UnaryNodeOutput<op> m_output;

        virtual void _initialize() {
            /* void */
        }

        virtual void registerInputs() {
            registerInput(m_output.getConnection(), "__in");
        }

        virtual void registerOutputs() {
            setPrimaryOutput(&m_output);
            registerOutput(&m_output, "__out");
        }
    };

    typedef UnaryNode<NEGATE>        NegateNode;
    typedef UnaryNode<NORMALIZE>    NormalizeNode;
    typedef UnaryNode<MAGNITUDE>    MagnitudeNode;

} /* namespace manta */

#endif /* MANTARAY_UNARY_NODE_H */
