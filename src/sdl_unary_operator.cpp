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

manta::SdlParserStructure *manta::SdlUnaryOperator::getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output) {
	SDL_RESET(query);

	SdlReferenceInfo basicInfo;
	SdlReferenceQuery basicQuery;
	basicQuery.inputContext = query.inputContext;
	basicQuery.recordErrors = false;
	SdlParserStructure *resolvedOperand = m_operand->getReference(basicQuery, &basicInfo);

	if (SDL_FAILED(&basicInfo) || resolvedOperand == nullptr) {
		SDL_FAIL();
		return nullptr;
	}

	if (basicInfo.touchedMainContext) SDL_INFO_OUT(touchedMainContext, true);

	bool isValidError = (SDL_EMPTY_CONTEXT() || basicInfo.touchedMainContext);

	if (m_operator == DEFAULT) {
		SdlParserStructure *result = resolvedOperand;

		if (basicInfo.reachedDeadEnd) {
			// This means that this references an input point with no default value. Obviously
			// it makes no sense to check for further errors.
			SDL_DEAD_END();
			return nullptr;
		}

		SdlNode *asNode = resolvedOperand->getAsNode();
		if (asNode != nullptr) result = asNode->getDefaultOutputValue();

		if (result == nullptr) {
			SDL_FAIL();

			if (query.recordErrors && isValidError) {
				// This object does not have a default value
				SDL_ERR_OUT(new SdlCompilationError(*m_operand->getSummaryToken(),
					SdlErrorCode::CannotFindDefaultValue, query.inputContext));
			}

			return nullptr;
		}

		if (query.inputContext != nullptr) SDL_INFO_OUT(newContext, query.inputContext->newChild(asNode));

		return result;
	}

	// Shouldn't reach here
	SDL_FAIL();
	return nullptr;
}

manta::NodeOutput *manta::SdlUnaryOperator::_generateNodeOutput(SdlContextTree *context, NodeProgram *program) {
	SdlValue *resolvedOperand = m_operand;

	if (resolvedOperand == nullptr) return nullptr;

	Node *node = generateNode(context, program);
	SdlNode *asNode = resolvedOperand->getAsNode();
	SdlValue *value = resolvedOperand->getAsValue();
	
	if (m_operator == DEFAULT) {
		return value->generateNode(context, program)->getPrimaryOutput();
	}

	return nullptr;
}

manta::Node *manta::SdlUnaryOperator::_generateNode(SdlContextTree *context, NodeProgram *program) {
	SdlValue *resolvedOperand = m_operand;

	if (resolvedOperand == nullptr) return nullptr;

	SdlValue *value = resolvedOperand->getAsValue();
	Node *node = value->generateNode(context, program);

	if (m_operator == DEFAULT) {
		return nullptr;
	}

	return nullptr;
}
