#include <sdl_binary_operator.h>

#include <sdl_value_constant.h>

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

void manta::SdlBinaryOperator::resolveReferences() {
	SdlValue::resolveReferences();

	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return;
	}

	if (m_operator == DOT) {
		// The dot is the reference operator
		if (!m_leftOperand->isResolved()) {
			m_leftOperand->resolveReferences();
		}

		SdlParserStructure *resolvedLeft = m_leftOperand->getReference();
		if (resolvedLeft == nullptr) {
			// Reference could not be resolved, skip the rest
			return;
		}

		// Check that the right operand is of the right type
		if (m_rightOperand->getType() != SdlValue::CONSTANT_LABEL) {
			// TODO: ERROR, invalid right hand operator
			return;
		}

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(resolvedLeft);
		SdlParserStructure *publicAttribute = resolvedLeft->getPublicAttribute(labelConstant->getValue());

		if (publicAttribute == nullptr) {
			// TODO: ERROR, left hand does not have this member
			return;
		}

		if (!publicAttribute->isResolved()) {
			publicAttribute->resolveReferences();
		}

		m_reference = publicAttribute->getReference();
	}
}
