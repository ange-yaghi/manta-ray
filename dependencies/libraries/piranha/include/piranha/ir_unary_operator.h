#ifndef PIRANHA_IR_UNARY_OPERATOR_H
#define PIRANHA_IR_UNARY_OPERATOR_H

#include "ir_value.h"

namespace piranha {

	class IrCompilationError;

	class IrUnaryOperator : public IrValue {
	public:
		enum OPERATOR {
			DEFAULT,
			NUM_NEGATE,
			BOOL_NEGATE
		};

	public:
		IrUnaryOperator(OPERATOR op, IrValue *operand);
		~IrUnaryOperator();

		OPERATOR getOperator() const { return m_operator; }
		IrValue *getOperand() const { return m_operand; }

		virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output);

	protected:
		OPERATOR m_operator;
		IrValue *m_operand;

	protected:
		virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program);
		virtual Node *_generateNode(IrContextTree *context, NodeProgram *program);
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_UNARY_OPERATOR_H */

