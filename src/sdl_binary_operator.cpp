#include <sdl_binary_operator.h>

#include <sdl_value_constant.h>
#include <node.h>

manta::SdlBinaryOperator::SdlBinaryOperator(OPERATOR op, SdlValue *left, SdlValue *right) :
							SdlValue(SdlValue::BINARY_OPERATION) {
	m_operator = op;
	m_leftOperand = left;
	m_rightOperand = right;

	registerComponent(left);
	registerComponent(right);

	// The data access operators are special in the sense that the right-hand operand
	// will *always* be a standard label that doesn't reference anything itself.
	// Therefore it wouldn't be wise to resolve the reference of that label
	if (op == DOT || op == POINTER) {
		m_rightOperand->setCheckReferences(false);
	}
}

manta::SdlBinaryOperator::~SdlBinaryOperator() {
	/* void */
}

manta::SdlParserStructure *manta::SdlBinaryOperator::getImmediateReference(
				SdlParserStructure *inputContext, SdlCompilationError **err) {
	if (err != nullptr) *err = nullptr;
	
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlParserStructure *resolvedLeft = nullptr;
		resolvedLeft = m_leftOperand->getReference();

		// Even if the left operand refers to an input, a reference to it should still
		// be found
		if (resolvedLeft == nullptr) {
			// Reference could not be resolved, skip the rest
			return nullptr;
		}

		// Check the input context for this symbol
		bool foundInput = false;
		if (inputContext != nullptr) {
			SdlParserStructure *inputConnection = m_leftOperand->getReference(inputContext);
			if (inputConnection != nullptr && inputConnection != resolvedLeft) {
				foundInput = true;
				resolvedLeft = inputConnection;
			}
		}

		bool isValidError = (inputContext == nullptr || (inputContext != nullptr && foundInput));

		// Perform code specific to the pointer operator
		if (m_operator == POINTER) {
			SdlNode *asNode = resolvedLeft->getAsNode();
			SdlValue *asValue = resolvedLeft->getAsValue();

			if (asValue != nullptr) resolvedLeft = asValue;
			else if (asNode != nullptr) resolvedLeft = asNode->getDefaultOutputValue();

			if (resolvedLeft == nullptr) {
				if (err != nullptr && isValidError) {
					*err = new SdlCompilationError(*m_leftOperand->getSummaryToken(),
						SdlErrorCode::CannotFindDefaultValue, inputContext);
				}

				return nullptr;
			}
		}

		// No sense in checking an input point for a member. If this is called it means that
		// the input in question has no default value and has returned itself as the object
		// it's referencing
		if (resolvedLeft->isInputPoint()) {
			return nullptr;
		}
		
		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);
		SdlParserStructure *publicAttribute = resolvedLeft->resolveLocalName(labelConstant->getValue());

		if (err != nullptr && publicAttribute == nullptr) {
			if (isValidError) {
				// Left hand does not have this member
				*err = new SdlCompilationError(*m_rightOperand->getSummaryToken(),
					SdlErrorCode::UndefinedMember, inputContext);
			}

			return nullptr;
		}

		// Check to make sure that the user is not accidentally trying to use a hidden member
		if (err != nullptr && !publicAttribute->allowsExternalAccess()) {
			*err = new SdlCompilationError(*m_rightOperand->getSummaryToken(),
				SdlErrorCode::AccessingInternalMember, inputContext);
			return nullptr;
		}

		return publicAttribute;
	}

	return nullptr;
}

manta::NodeOutput *manta::SdlBinaryOperator::_generateNodeOutput(SdlParserStructure *context) {
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlValue *value = m_leftOperand;

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);

		const NodeOutput *leftOutput = m_leftOperand->generateNodeOutput(context);
		const Node *leftNode = m_leftOperand->generateNode(context);

		if (leftOutput == nullptr && leftNode == nullptr) return nullptr;
		if (leftNode != nullptr) {
			if (m_operator == DOT) {
				return leftNode->getOutput(labelConstant->getValue().c_str());
			}
			else if (m_operator == POINTER) {
				NodeOutput *primaryOutput = leftNode->getPrimaryOutput();
				Node *outputInterface = primaryOutput->getInterface();
				NodeOutput *output = outputInterface->getOutput(labelConstant->getValue().c_str());
				return leftNode
					->getPrimaryOutput()
					->getInterface()
					->getOutput(labelConstant->getValue().c_str());
			}
		}

		if (leftOutput != nullptr) {
			if (m_operator == DOT) {
				return leftOutput->getInterface()->getOutput(labelConstant->getValue().c_str());
			}
			else if (m_operator == POINTER) {
				return leftOutput->getInterface()->getPrimaryOutput()->getInterface()->getOutput(labelConstant->getValue().c_str());
			}
		}
	}

	// TODO: add other operators
	return nullptr;
}

manta::Node *manta::SdlBinaryOperator::_generateNode(SdlParserStructure *context) {
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlParserStructure *reference = getReference(context);
		SdlNode *asNode = reference->getAsNode();

		if (asNode != nullptr) {
			return asNode->generateNode();
		}
	}

	// TODO: add other operators
	return nullptr;
}
