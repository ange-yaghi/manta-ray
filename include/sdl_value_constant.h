#ifndef SDL_VALUE_CONSTANT_H
#define SDL_VALUE_CONSTANT_H

#include <sdl_value.h>

#include <sdl_token_info.h>
#include <sdl_compilation_error.h>
#include <sdl_compilation_unit.h>
#include <sdl_node.h>
#include <single_float_node_output.h>
#include <standard_allocator.h>

namespace manta {

	class SdlNode;

	template <typename T, SdlValue::VALUE_TYPE TypeCode>
	class SdlValueConstant : public SdlValue {
	protected:
		typedef T_SdlTokenInfo<T> _TokenInfo;

		static NodeOutput *generateNodeOutput(math::real_d value) {
			SingleFloatNodeOutput *newNode = StandardAllocator::Global()->allocate<SingleFloatNodeOutput>();
			newNode->setValue(value);

			return newNode;
		}

		static NodeOutput *generateNodeOutput(const std::string &value) {
			// TODO
			return nullptr;
		}

		static NodeOutput *generateNodeOutput(SdlNode *node) {
			// TODO
			return nullptr;
		}

		static NodeOutput *generateNodeOutput(bool value) {
			// TODO
			return nullptr;
		}

		static NodeOutput *generateNodeOutput(int value) {
			// TODO
			return nullptr;
		}

	public:
		SdlValueConstant(const _TokenInfo &value) : SdlValue(TypeCode) { m_value = value.data; useToken(value); }
		virtual ~SdlValueConstant() { /* void */ }

		const _TokenInfo *getToken() const { return &m_token; }
		void useToken(const _TokenInfo &info) { m_value = info.data; registerToken(&info); }

		virtual void setValue(const T &value) { m_value = value; }
		T getValue() const { return m_value; }

		virtual NodeOutput *generateNodeOutput() {
			return generateNodeOutput(m_value);
		}

	protected:
		T m_value;
		_TokenInfo m_token;
	};

	// Specialized type for labels
	class SdlValueLabel : public SdlValueConstant<std::string, SdlValue::CONSTANT_LABEL> {
	public:
		SdlValueLabel(const _TokenInfo &value) : SdlValueConstant(value) { /* void */ }
		~SdlValueLabel() { /* void */ }

		virtual SdlParserStructure *getImmediateReference(SdlParserStructure *inputContext, SdlCompilationError **err = nullptr) {
			SdlParserStructure *reference = nullptr;

			// First check the input context for the reference
			if (inputContext != nullptr) {
				reference = inputContext->resolveLocalName(m_value);
			}

			if (reference == nullptr) {
				reference = resolveName(m_value);
			}

			// Do error checking
			if (err != nullptr) {
				if (reference == nullptr && inputContext == nullptr) {
					*err = new SdlCompilationError(m_summaryToken,
						SdlErrorCode::UnresolvedReference, inputContext);
				}
				else {
					*err = nullptr;
				}
			}

			return reference;
		}
	};

	// Specialized type for node references
	class SdlValueNodeRef : public SdlValueConstant<SdlNode *, SdlValue::NODE_REF> {
	public:
		SdlValueNodeRef(const _TokenInfo &value) : SdlValueConstant(value) { registerComponent(value.data); }
		~SdlValueNodeRef() { /* void */ }

		virtual void setValue(SdlNode *const &value) {
			m_value = value; 
			registerComponent(value); 
		}

		virtual SdlParserStructure *getImmediateReference(SdlParserStructure *inputContext, SdlCompilationError **err = nullptr) {
			(void)inputContext;

			if (err != nullptr) *err = nullptr;
			return m_value;
		}
	};

	typedef SdlValueConstant<int, SdlValue::CONSTANT_INT> SdlValueInt;
	typedef SdlValueConstant<std::string, SdlValue::CONSTANT_STRING> SdlValueString;
	typedef SdlValueConstant<double, SdlValue::CONSTANT_FLOAT> SdlValueFloat;
	typedef SdlValueConstant<bool, SdlValue::CONSTANT_BOOL> SdlValueBool;

} /* namespace manta */

#endif /* SDL_VALUE_CONSTANT_H */
