#ifndef PIRANHA_BOOL_NEGATE_OPERATION_H
#define PIRANHA_BOOL_NEGATE_OPERATION_H

#include "fundamental_output.h"

#include "pipe_node.h"

namespace piranha {

    template <typename Type>
    class BoolNegateOperationNodeOutput : public FundamentalOutput<Type> {
    public:
        BoolNegateOperationNodeOutput() {
            /* void */
        }

        virtual ~BoolNegateOperationNodeOutput() {
            /* void */
        }

        virtual void fullCompute(void *_target) const {
            Type *target = reinterpret_cast<Type *>(_target);

            Type in;
            m_input->fullCompute(&in);

            *target = !in;
        }

        pNodeInput *getConnection() { return &m_input; }

    protected:
        virtual void registerInputs() {
            this->registerInput(&m_input);
        }

    protected:
        pNodeInput m_input;
    };

    template <typename Type>
    using BoolNegateOperationNode = PipeNode<BoolNegateOperationNodeOutput<Type>>;

} /* namespace piranha */

#endif /* PIRANHA_BOOL_NEGATE_OPERATION_H */
