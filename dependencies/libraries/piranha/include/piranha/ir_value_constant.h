#ifndef PIRANHA_IR_VALUE_CONSTANT_H
#define PIRANHA_IR_VALUE_CONSTANT_H

#include "ir_value.h"

#include "ir_token_info.h"
#include "ir_compilation_error.h"
#include "ir_compilation_unit.h"
#include "ir_node.h"
#include "ir_context_tree.h"
#include "single_float_node_output.h"
#include "single_string_node_output.h"
#include "standard_allocator.h"
#include "node.h"

namespace piranha {

	class IrNode;

	template <typename T, IrValue::VALUE_TYPE TypeCode>
	class IrValueConstant : public IrValue {
	protected:
		typedef T_IrTokenInfo<T> _TokenInfo;

		static NodeOutput *generateNodeOutput(double value, IrContextTree *context) {
			SingleFloatNodeOutput *newNode = StandardAllocator::Global()->allocate<SingleFloatNodeOutput>();
			newNode->setValue(value);

			return newNode;
		}

		static NodeOutput *generateNodeOutput(const std::string &value, IrContextTree *context) {
			SingleStringNodeOutput *newNode = StandardAllocator::Global()->allocate<SingleStringNodeOutput>();
			newNode->setValue(value);

			return newNode;
		}

		static NodeOutput *generateNodeOutput(bool value, IrContextTree *context) {
			// TODO
			return nullptr;
		}

		static NodeOutput *generateNodeOutput(int value, IrContextTree *context) {
			// TODO
			return nullptr;
		}

	public:
		IrValueConstant(const _TokenInfo &value) : IrValue(TypeCode) { m_value = value.data; useToken(value); }
		virtual ~IrValueConstant() { /* void */ }

		const _TokenInfo *getToken() const { return &m_token; }
		void useToken(const _TokenInfo &info) { m_value = info.data; registerToken(&info); }

		virtual void setValue(const T &value) { m_value = value; }
		T getValue() const { return m_value; }

		virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program) {
			return generateNodeOutput(m_value, context);
		}

	protected:
		T m_value;
		_TokenInfo m_token;
	};

	// Specialized type for labels
	class IrValueLabel : public IrValueConstant<std::string, IrValue::CONSTANT_LABEL> {
	public:
		IrValueLabel(const _TokenInfo &value) : IrValueConstant(value) { /* void */ }
		~IrValueLabel() { /* void */ }

		virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output) {
			IR_RESET(query);

			IrParserStructure *reference = resolveName(m_value);

			// Do error checking
			if (reference == nullptr) {
				IR_FAIL();

				if (query.recordErrors && IR_EMPTY_CONTEXT()) {
					IR_ERR_OUT(new IrCompilationError(m_summaryToken,
						IrErrorCode::UnresolvedReference, query.inputContext));
				}

				return nullptr;
			}	

			return reference;
		}

		virtual Node *_generateNode(IrContextTree *context, NodeProgram *program) {
			IrReferenceInfo info;
			IrReferenceQuery query;
			query.inputContext = context;
			query.recordErrors = false;
			IrParserStructure *reference = getReference(query, &info);
			if (reference == nullptr) return nullptr;

			IrNode *asNode = reference->getAsNode();
			if (asNode != nullptr) {
				return asNode->generateNode(info.newContext, program);
			}
			else return nullptr;
		}

		virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program) {
			IrReferenceInfo info;
			IrReferenceQuery query;
			query.inputContext = context;
			query.recordErrors = false;
			IrParserStructure *reference = getReference(query, &info);
			if (reference == nullptr) return nullptr;

			IrNode *asNode = reference->getAsNode();
			if (asNode != nullptr) {
				Node *generatedNode = asNode->generateNode(info.newContext, program);
				if (generatedNode != nullptr) {
					return generatedNode->getPrimaryOutput();
				}
			}

			return reference->getAsValue()->generateNodeOutput(info.newContext, program);
		}
	};

	// Specialized type for node references
	class IrValueNodeRef : public IrValueConstant<IrNode *, IrValue::NODE_REF> {
	public:
		IrValueNodeRef(const _TokenInfo &value) : IrValueConstant(value) { registerComponent(value.data); }
		~IrValueNodeRef() { /* void */ }

		virtual void setValue(IrNode *const &value) {
			m_value = value; 
			registerComponent(value); 
		}

		virtual IrParserStructure *getImmediateReference(const IrReferenceQuery &query, IrReferenceInfo *output) {
			IR_RESET(query);

			return m_value;
		}

		virtual IrNode *asNode() {
			return m_value;
		}

		virtual Node *_generateNode(IrContextTree *context, NodeProgram *program) {
			return m_value->generateNode(context, program);
		}

		virtual NodeOutput *_generateNodeOutput(IrContextTree *context, NodeProgram *program) {
			return nullptr;
		}
	};

	typedef IrValueConstant<int, IrValue::CONSTANT_INT> IrValueInt;
	typedef IrValueConstant<std::string, IrValue::CONSTANT_STRING> IrValueString;
	typedef IrValueConstant<double, IrValue::CONSTANT_FLOAT> IrValueFloat;
	typedef IrValueConstant<bool, IrValue::CONSTANT_BOOL> IrValueBool;

} /* namespace piranha */

#endif /* PIRANHA_IR_VALUE_CONSTANT_H */
