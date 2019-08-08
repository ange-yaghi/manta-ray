#ifndef MANTARAY_BINARY_NODE_H
#define MANTARAY_BINARY_NODE_H

#include "node.h"

#include "binary_node_output.h"

namespace manta {
    
    template <BINARY_OPERATION op>
    class BinaryNode : public Node {
    public:
        BinaryNode() {
            /* void */
        }

        virtual ~BinaryNode() {
            /* void */
        }

    protected:
        BinaryNodeOutput<op> m_output;

        virtual void _initialize() {
            m_output.initialize();
        }

        virtual void registerInputs() {
            registerInput(m_output.getLeftConnection(), "__left");
            registerInput(m_output.getRightConnection(), "__right");
        }

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutput(&m_output, "__out");
        }
    };

    typedef BinaryNode<SUB>         SubtractNode;
    typedef BinaryNode<ADD>         AddNode;
    typedef BinaryNode<MUL>         MultiplyNode;
    typedef BinaryNode<DIV>         DivideNode;
    typedef BinaryNode<CROSS>       CrossNode;
    typedef BinaryNode<DOT>         DotNode;
    typedef BinaryNode<POW>         PowerNode;
    typedef BinaryNode<MAX>         MaxNode;
    typedef BinaryNode<MIN>         MinNode;

} /* namespace manta */

#endif /* MANTARAY_BINARY_NODE_H */
