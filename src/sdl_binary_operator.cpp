#include <sdl_binary_operator.h>

#include <sdl_value_constant.h>

manta::SdlBinaryOperator::SdlBinaryOperator(OPERATOR op, SdlValue *left, SdlValue *right) :
							SdlValue(SdlValue::BINARY_OPERATION) {
	m_operator = op;
	m_leftOperand = left;
	m_rightOperand = right;

	registerComponent(left);
	registerComponent(right);

	// The dot operator is special in the sense that the right-hand operand
	// will *always* be a standard label that doesn't reference anything itself.
	// Therefore it wouldn't be wise to resolve the reference of that label
	m_rightOperand->setCheckReferences(false);
}

manta::SdlBinaryOperator::~SdlBinaryOperator() {
	/* void */
}

manta::SdlParserStructure *manta::SdlBinaryOperator::getImmediateReference(SdlCompilationError **err) {
	if (err != nullptr) *err = nullptr;
	
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT) {
		SdlParserStructure *resolvedLeft = m_leftOperand->getReference();
		if (resolvedLeft == nullptr) {
			// Reference could not be resolved, skip the rest
			return nullptr;
		}

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);
		SdlParserStructure *publicAttribute = resolvedLeft->resolveLocalName(labelConstant->getValue());

		if (err != nullptr && publicAttribute == nullptr) {
			// Left hand does not have this member
			*err = new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::UndefinedMember);
			return nullptr;
		}

		// Check to make sure that the user is not accidentally trying to use a hidden member
		if (err != nullptr && !publicAttribute->allowsExternalAccess()) {
			*err = new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::AccessingInternalMember);
			return nullptr;
		}

		return publicAttribute;
	}

	// Todo: Expansion code would go here
	return nullptr;
}
