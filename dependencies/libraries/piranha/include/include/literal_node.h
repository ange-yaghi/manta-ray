#ifndef PIRANHA_LITERAL_NODE_H
#define PIRANHA_LITERAL_NODE_H

#include "node.h"

#include "literal_node_output.h"

namespace piranha {

    template <typename LiteralType>
    class LiteralNode : public Node {
    public:
        LiteralNode() {}
        ~LiteralNode() {}

        virtual void setData(LiteralType data) = 0;
    };

    typedef LiteralNode<piranha::native_string> StringLiteralNode;
    typedef LiteralNode<piranha::native_int> IntLiteralNode;
    typedef LiteralNode<piranha::native_float> FloatLiteralNode;
    typedef LiteralNode<piranha::native_bool> BoolLiteralNode;

} /* namespace piranha */

#endif /* PIRANHA_LITERAL_NODE_H */
