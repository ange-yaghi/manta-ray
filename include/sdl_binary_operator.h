#ifndef SDL_BINARY_OPERATOR_H
#define SDL_BINARY_OPERATOR_H

#include <sdl_value.h>

namespace manta {

	class SdlBinaryOperator : public SdlValue {
	public:
		enum OPERATOR {
			DOT,
			ADD,
			SUB,
			MUL,
			DIV
		};

	public:
		SdlBinaryOperator(OPERATOR op, SdlValue *leftOperand, SdlValue *rightOperand);
		~SdlBinaryOperator();

		OPERATOR getOperator() const { return m_operator; }
		SdlValue *getLeft() const { return m_leftOperand; }
		SdlValue *getRight() const { return m_rightOperand; }

	protected:
		OPERATOR m_operator;
		SdlValue *m_leftOperand;
		SdlValue *m_rightOperand;

		// Resolution stage
	public:
		virtual void resolveReferences();
	};

} /* namespace manta */

#endif /* SDL_BINARY_OPERATOR_H */
