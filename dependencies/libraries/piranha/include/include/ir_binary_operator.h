#ifndef PIRANHA_IR_BINARY_OPERATOR_H
#define PIRANHA_IR_BINARY_OPERATOR_H

#include "ir_value.h"

#include "pkey_value_lookup.h" 

namespace piranha {

    class CompilationError;
    class IrNodeDefinition;

    class IrBinaryOperator : public IrValue {
    public:
        enum class Operator {
            Dot,
            Add,
            Sub,
            Mul,
            Div
        };

    public:
        IrBinaryOperator(Operator op, IrValue *leftOperand, IrValue *rightOperand);
        ~IrBinaryOperator();

        Operator getOperator() const { return m_operator; }
        IrValue *getLeft() const { return m_leftOperand; }
        IrValue *getRight() const { return m_rightOperand; }

        virtual IrParserStructure *resolveLocalName(const std::string &name) const;
        virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output);

    protected:
        void _expand(IrContextTree *tree);

    protected:
        Operator m_operator;
        IrValue *m_leftOperand;
        IrValue *m_rightOperand;

    protected:
        virtual Node *_generateNode(IrContextTree *context, NodeProgram *program, NodeContainer *container);
        virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program, NodeContainer *container);
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_BINARY_OPERATOR_H */
