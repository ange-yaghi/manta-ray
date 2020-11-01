#ifndef PIRANHA_DEFAULT_LITERAL_NODE_H
#define PIRANHA_DEFAULT_LITERAL_NODE_H

#include "literal_node.h"

namespace piranha {

    template <typename LiteralType>
    class DefaultLiteralNode : public LiteralNode<LiteralType> {
    public:
        DefaultLiteralNode() {
            /* void */
        }

        ~DefaultLiteralNode() {
            /* void */
        }

        virtual void setData(LiteralType data) { m_literalData = data; }

    protected:
        virtual void _initialize() {
            /* void */
        }

        virtual Node *_optimize(NodeAllocator *nodeAllocator) {
            Node::addFlag(Node::META_CONSTANT);
            Node::addFlag(Node::META_ACTIONLESS);

            return this;
        }

        virtual void _evaluate() {
            m_output.setData(m_literalData);
        }

        virtual void registerOutputs() {
            Node::setPrimaryOutput("__out");
            Node::registerOutput(&m_output, "__out");
        }

    protected:
        LiteralNodeOutput<LiteralType> m_output;
        LiteralType m_literalData;
    };

    typedef DefaultLiteralNode<piranha::native_string> DefaultLiteralStringNode;
    typedef DefaultLiteralNode<piranha::native_int> DefaultLiteralIntNode;
    typedef DefaultLiteralNode<piranha::native_float> DefaultLiteralFloatNode;
    typedef DefaultLiteralNode<piranha::native_bool> DefaultLiteralBoolNode;

} /* namespace piranha */

#endif /* PIRANHA_DEFAULT_LITERAL_NODE_H */
