#ifndef PIRANHA_NO_OP_NODE_H
#define PIRANHA_NO_OP_NODE_H

#include "node.h"

namespace piranha {

    class NoOpNode : public Node {
    public:
        NoOpNode() {
            /* void */
        }

        ~NoOpNode() {
            /* void */
        }

        virtual void _initialize() {
            /* void */
        }

        virtual void _evaluate() {
            /* void */
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutputReference(&m_input, "__out");
        }

        virtual void registerInputs() {
            registerInput(&m_input, "__in");
        }

    protected:
        pNodeInput m_input;
    };

} /* namespace piranha */

#endif /* PIRANHA_NO_OP_NODE_H */
