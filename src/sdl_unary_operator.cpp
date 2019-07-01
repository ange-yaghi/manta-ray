#include <sdl_unary_operator.h>

#include <sdl_compilation_error.h>
#include <sdl_node.h>
#include <node.h>
#include <sdl_context_tree.h>

manta::SdlUnaryOperator::SdlUnaryOperator(OPERATOR op, SdlValue *operand) : SdlValue(SdlValue::UNARY_OPERATION) {
	m_operand = operand;
	m_operator = op;

	registerComponent(operand);
}

manta::SdlUnaryOperator::~SdlUnaryOperator() {
	/* void */
}

manta::SdlParserStructure *manta::SdlUnaryOperator::getImmediateReference(SdlContextTree *inputContext, SdlCompilationError **err, SdlContextTree **newContext) {
	if (err != nullptr) *err = nullptr;
	if (newContext != nullptr) *newContext = inputContext;

	SdlParserStructure *resolvedOperand = m_operand->getReference();

	if (resolvedOperand == nullptr) {
		return nullptr;
	}

	// Check the input context
	bool foundInput = false;
	if (inputContext != nullptr) {
		SdlParserStructure *inputConnection = resolvedOperand->getReference(inputContext, nullptr, newContext);
		if (inputConnection != nullptr && inputConnection != resolvedOperand) {
			foundInput = true;
			resolvedOperand = inputConnection;
		}
	}

	if (m_operator == DEFAULT) {
		SdlParserStructure *result = resolvedOperand->getAsValue();
		SdlNode *asNode = resolvedOperand->getAsNode();
		if (result == nullptr && asNode != nullptr) result = asNode->getDefaultOutputValue();

		if (err != nullptr && result == nullptr) {
			bool isValidError = (inputContext == nullptr || (inputContext != nullptr && foundInput));

			if (isValidError) {
				// This object does not have a default value
				*err = new SdlCompilationError(*m_operand->getSummaryToken(),
					SdlErrorCode::CannotFindDefaultValue, inputContext);
			}

			return nullptr;
		}

		return result;
	}

	return nullptr;
}

manta::NodeOutput *manta::SdlUnaryOperator::_generateNodeOutput(SdlContextTree *context) {
	SdlValue *resolvedOperand = m_operand;

	if (resolvedOperand == nullptr) return nullptr;

	Node *node = generateNode(context);
	SdlNode *asNode = resolvedOperand->getAsNode();
	SdlValue *value = resolvedOperand->getAsValue();
	
	if (m_operator == DEFAULT) {
		return value->generateNode(context)->getPrimaryOutput();
	}

	return nullptr;
}

manta::Node *manta::SdlUnaryOperator::_generateNode(SdlContextTree *context) {
	SdlValue *resolvedOperand = m_operand;

	if (resolvedOperand == nullptr) return nullptr;

	SdlValue *value = resolvedOperand->getAsValue();
	Node *node = value->generateNode(context);

	if (m_operator == DEFAULT) {
		return nullptr;
	}

	return nullptr;
}
