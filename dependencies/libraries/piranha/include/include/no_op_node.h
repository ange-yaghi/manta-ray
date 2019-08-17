#ifndef PIRANHA_NO_OP_NODE_H
#define PIRANHA_NO_OP_NODE_H

#include "node.h"

namespace piranha {

    class NoOpNode : public Node {
    public:
        NoOpNode();
        ~NoOpNode();

        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

    protected:
        pNodeInput m_input;
    };

} /* namespace piranha */

#endif /* PIRANHA_NO_OP_NODE_H */
