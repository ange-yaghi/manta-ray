#include <sdl_binary_operator.h>

manta::SdlBinaryOperator::SdlBinaryOperator(OPERATOR op, SdlValue *left, SdlValue *right) :
							SdlValue(SdlValue::BINARY_OPERATION) {
	m_operator = op;
	m_leftOperand = left;
	m_rightOperand = right;

	registerComponent(left);
	registerComponent(right);
}

manta::SdlBinaryOperator::~SdlBinaryOperator() {
	/* void */
}
