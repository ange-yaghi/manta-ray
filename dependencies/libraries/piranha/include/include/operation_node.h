#ifndef PIRANHA_OPERATION_NODE_H
#define PIRANHA_OPERATION_NODE_H

#include "node.h"

#include "add_operation_output.h"
#include "fundamental_output.h"

namespace piranha {

    class OperationNode : public Node {
    public:
        OperationNode() {}
        ~OperationNode() {}

        virtual void connectLeft(pNodeInput input) {
            (*m_inputs[0].input) = input;
        }

        virtual void connectRight(pNodeInput input) {
            (*m_inputs[1].input) = input;
        }
    };

    template <typename FundamentalType, template<class> class OperationNodeType>
    class OperationNodeSpecialized : public OperationNode {
    public:
        OperationNodeSpecialized() {
            /* void */
        }

        ~OperationNodeSpecialized() {
            /* void */
        }

        virtual void _initialize() {
            m_output.initialize();
        }

        virtual void _evaluate() {
            /* void */
        }

        virtual void _destroy() {
            /* void */
        }

        virtual void registerOutputs() {
            setPrimaryOutput("__out");
            registerOutput(&m_output, "__out");
        }

        virtual void registerInputs() {
            registerInput(
                m_output.getLeftConnection(), "__in0");
            registerInput(
                m_output.getRightConnection(), "__in1");
        }

    protected:
        OperationNodeType<FundamentalType> m_output;
    };

} /* namespace piranha */

#endif /* PIRANHA_OPERATION_NODE_H */
