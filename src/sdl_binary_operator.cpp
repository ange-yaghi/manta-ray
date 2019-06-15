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
	if (m_operator == DOT) {
		m_resolveReferencesChildren = false;
	}
}

manta::SdlBinaryOperator::~SdlBinaryOperator() {
	/* void */
}

void manta::SdlBinaryOperator::_resolveReferences(SdlCompilationUnit *unit) {
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return;
	}

	// The dot is the reference operator
	if (m_operator == DOT) {
		// Make sure that the left operand is actually resolved
		m_leftOperand->resolveReferences(unit);

		SdlParserStructure *resolvedLeft = m_leftOperand->getReference();
		if (resolvedLeft == nullptr) {
			// Reference could not be resolved, skip the rest
			return;
		}

		// Check that the right operand is of the right type
		if (m_rightOperand->getType() != SdlValue::CONSTANT_LABEL) {
			unit->addCompilationError(new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::InvalidRightHandOperand));
			return;
		}

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);
		SdlParserStructure *publicAttribute = resolvedLeft->resolveLocalName(labelConstant->getValue());

		// Check to make sure that the user is not accidentally trying to use a hidden member
		if (!publicAttribute->allowsExternalAccess()) {
			unit->addCompilationError(new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::AccessingInternalMember));
			return;
		}

		if (publicAttribute == nullptr) {
			// Left hand does not have this member
			unit->addCompilationError(new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::UndefinedMember));
			return;
		}

		m_reference = publicAttribute;
	}
	else {
		// Other operators need both right and left hand to be resolved
		m_leftOperand->resolveReferences(unit);
		m_rightOperand->resolveReferences(unit);
	}
}
