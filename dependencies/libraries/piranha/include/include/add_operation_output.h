#ifndef PIRANHA_ADD_OPERATION_OUTPUT_H
#define PIRANHA_ADD_OPERATION_OUTPUT_H

#include "fundamental_output.h"

#include "pipe_node.h"
#include "default_literal_node.h"

namespace piranha {

    template <typename Type>
    class AddOperationNodeOutput : public FundamentalOutput<Type> {
    public:
        AddOperationNodeOutput() {
            /* void */
        }

        virtual ~AddOperationNodeOutput() {
            /* void */
        }

        virtual void fullCompute(void *_target) const {
            Type *target = reinterpret_cast<Type *>(_target);

            Type left;
            Type right;

            m_left->fullCompute((void *)&left);
            m_right->fullCompute((void *)&right);
            *target = left + right;
        }

        pNodeInput *getLeftConnection() { return &m_left; }
        pNodeInput *getRightConnection() { return &m_right; }

    protected:
        virtual void registerInputs() {
            this->registerInput(&m_left);
            this->registerInput(&m_right);
        }

    protected:
        pNodeInput m_left;
        pNodeInput m_right;
    };

} /* namespace piranha */

#endif /* PIRANHA_ADD_OPERATION_OUTPUT_H */
