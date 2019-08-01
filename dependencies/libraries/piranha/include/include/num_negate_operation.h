#ifndef PIRANHA_NUM_NEGATE_OPERATION_H
#define PIRANHA_NUM_NEGATE_OPERATION_H

#include "fundamental_output.h"

#include "pipe_node.h"

namespace piranha {

    template <typename Type>
    class NumNegateOperationNodeOutput : public FundamentalOutput<Type> {
    public:
        NumNegateOperationNodeOutput() {
            /* void */
        }

        virtual ~NumNegateOperationNodeOutput() {
            /* void */
        }

        virtual void fullCompute(void *_target) const {
            Type *target = reinterpret_cast<Type *>(_target);

            Type in;
            m_input->fullCompute(&in);

            *target = -in;
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
    using NumNegateOperationNode = PipeNode<NumNegateOperationNodeOutput<Type>>;

} /* namespace piranha */

#endif /* PIRANHA_NUM_NEGATE_OPERATION_H */
