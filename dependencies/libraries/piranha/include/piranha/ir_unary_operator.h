#ifndef PIRANHA_IR_UNARY_OPERATOR_H
#define PIRANHA_IR_UNARY_OPERATOR_H

#include "ir_value.h"

namespace piranha {

    class CompilationError;

    class IrUnaryOperator : public IrValue {
    public:
        enum OPERATOR {
            DEFAULT,
            NUM_NEGATE,
            BOOL_NEGATE,
            POSITIVE
        };

    public:
        IrUnaryOperator(OPERATOR op, IrValue *operand);
        ~IrUnaryOperator();

        OPERATOR getOperator() const { return m_operator; }
        IrValue *getOperand() const { return m_operand; }

        virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output);

    protected:
        void _expand(IrContextTree *context);

    protected:
        OPERATOR m_operator;
        IrValue *m_operand;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_UNARY_OPERATOR_H */

