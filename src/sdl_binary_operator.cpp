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
		if (m_rightOperand != nullptr) {
			m_rightOperand->setCheckReferences(false);
		}
	}
}

manta::SdlBinaryOperator::~SdlBinaryOperator() {
	/* void */
}

manta::SdlParserStructure *manta::SdlBinaryOperator::getImmediateReference(
					const SdlReferenceQuery &query, SdlReferenceInfo *output) {
	SDL_RESET(query);
	
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		SDL_FAIL();
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlReferenceQuery basicQuery;
		basicQuery.inputContext = query.inputContext;
		basicQuery.recordErrors = false;
		SdlReferenceInfo basicInfo;
		SdlParserStructure *resolvedLeft = m_leftOperand->getReference(basicQuery, &basicInfo);

		if (basicInfo.failed) {
			SDL_FAIL();
			return nullptr;
		}

		if (basicInfo.reachedDeadEnd) {
			SDL_DEAD_END();
			return nullptr;
		}

		bool touchedMainContext = false;
		if (basicInfo.touchedMainContext) {
			SDL_INFO_OUT(touchedMainContext, true);
			touchedMainContext = true;
		}

		SdlContextTree *currentContext = basicInfo.newContext;

		// Perform code specific to the pointer operator
		if (m_operator == POINTER) {
			SdlNode *asNode = resolvedLeft->getAsNode();
			SdlValue *asValue = resolvedLeft->getAsValue();

			if (asValue != nullptr) resolvedLeft = asValue;
			else if (asNode != nullptr) resolvedLeft = asNode->getDefaultOutputValue();

			if (resolvedLeft == nullptr) {
				SDL_FAIL();

				bool isValidError = (SDL_EMPTY_CONTEXT() || touchedMainContext);
				if (query.recordErrors && isValidError) {
					SDL_ERR_OUT(new SdlCompilationError(*m_leftOperand->getSummaryToken(),
						SdlErrorCode::CannotFindDefaultValue, query.inputContext));
				}

				return nullptr;
			}

			if (currentContext != nullptr) {
				currentContext = currentContext->newChild(asNode);
			}

			SdlReferenceInfo refInfo;
			SdlReferenceQuery refQuery;
			refQuery.inputContext = currentContext;
			refQuery.recordErrors = false;
			resolvedLeft = resolvedLeft->getReference(refQuery, &refInfo);

			if (SDL_FAILED(&refInfo)) {
				SDL_FAIL();
				return nullptr;
			}

			if (refInfo.reachedDeadEnd) {
				SDL_DEAD_END();
				return nullptr;
			}

			if (refInfo.touchedMainContext) {
				SDL_INFO_OUT(touchedMainContext, true);
				touchedMainContext = true;
			}

			currentContext = refInfo.newContext;
		}

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);
		SdlParserStructure *publicAttribute = resolvedLeft->resolveLocalName(labelConstant->getValue());

		if (publicAttribute == nullptr) {
			SDL_FAIL();

			bool isValidError = (SDL_EMPTY_CONTEXT() || touchedMainContext);
			if (query.recordErrors && isValidError) {
				// Left hand does not have this member
				SDL_ERR_OUT(new SdlCompilationError(*m_rightOperand->getSummaryToken(),
					SdlErrorCode::UndefinedMember, query.inputContext));
			}

			return nullptr;
		}

		// Check to make sure that the user is not accidentally trying to use a hidden member
		if (!publicAttribute->allowsExternalAccess()) {
			SDL_FAIL();

			bool isValidError = (SDL_EMPTY_CONTEXT() || touchedMainContext);
			if (query.recordErrors && isValidError) {
				SDL_ERR_OUT(new SdlCompilationError(*m_rightOperand->getSummaryToken(),
					SdlErrorCode::AccessingInternalMember, query.inputContext));
			}

			return nullptr;
		}

		if (currentContext != nullptr) {
			SDL_INFO_OUT(newContext, currentContext->newChild(resolvedLeft->getAsNode()));
		}

		return publicAttribute;
	}

	return nullptr;
}

manta::NodeOutput *manta::SdlBinaryOperator::_generateNodeOutput(SdlContextTree *context, NodeProgram *program) {
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlValue *value = m_leftOperand;

		SdlValueLabel *labelConstant = static_cast<SdlValueLabel *>(m_rightOperand);

		const NodeOutput *leftOutput = m_leftOperand->generateNodeOutput(context, program);
		const Node *leftNode = m_leftOperand->generateNode(context, program);

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
				return leftOutput
					->getInterface()
					->getOutput(labelConstant->getValue().c_str());
			}
			else if (m_operator == POINTER) {
				return leftOutput
					->getInterface()
					->getPrimaryOutput()
					->getInterface()
					->getOutput(labelConstant->getValue().c_str());
			}
		}
	}

	// TODO: add other operators
	return nullptr;
}

manta::Node *manta::SdlBinaryOperator::_generateNode(SdlContextTree *context, NodeProgram *program) {
	if (m_leftOperand == nullptr || m_rightOperand == nullptr) {
		// There was a syntax error so this step can be skipped
		return nullptr;
	}

	// The dot is the reference operator
	if (m_operator == DOT || m_operator == POINTER) {
		SdlReferenceInfo info;
		SdlReferenceQuery query;
		query.inputContext = context;
		query.recordErrors = false;

		SdlParserStructure *reference = getReference(query, &info);
		SdlNode *asNode = reference->getAsNode();

		if (asNode != nullptr) {
			return asNode->generateNode(info.newContext, program);
		}
	}

	// TODO: add other operators
	return nullptr;
}
