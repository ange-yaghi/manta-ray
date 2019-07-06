#ifndef SDL_UNARY_OPERATOR_H
#define SDL_UNARY_OPERATOR_H

#include <sdl_value.h>

namespace manta {

	class SdlCompilationError;

	class SdlUnaryOperator : public SdlValue {
	public:
		enum OPERATOR {
			DEFAULT,
			NUM_NEGATE,
			BOOL_NEGATE
		};

	public:
		SdlUnaryOperator(OPERATOR op, SdlValue *operand);
		~SdlUnaryOperator();

		OPERATOR getOperator() const { return m_operator; }
		SdlValue *getOperand() const { return m_operand; }

		virtual SdlParserStructure *getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output);

	protected:
		OPERATOR m_operator;
		SdlValue *m_operand;

	protected:
		virtual NodeOutput *_generateNodeOutput(SdlContextTree *context, NodeProgram *program);
		virtual Node *_generateNode(SdlContextTree *context, NodeProgram *program);
	};

} /* namespace manta */

#endif /* SDL_UNARY_OPERATOR_H */

