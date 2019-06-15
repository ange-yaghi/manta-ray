#ifndef SDL_VALUE_CONSTANT_H
#define SDL_VALUE_CONSTANT_H

#include <sdl_value.h>

#include <sdl_token_info.h>
#include <sdl_compilation_error.h>
#include <sdl_compilation_unit.h>

namespace manta {

	class SdlNode;

	template <typename T, SdlValue::VALUE_TYPE TypeCode>
	class SdlValueConstant : public SdlValue {
	protected:
		typedef T_SdlTokenInfo<T> _TokenInfo;

	public:
		SdlValueConstant(const _TokenInfo &value) : SdlValue(TypeCode) { m_value = value.data; useToken(value); }
		virtual ~SdlValueConstant() { /* void */ }

		const _TokenInfo *getToken() const { return &m_token; }
		void useToken(const _TokenInfo &info) { m_value = info.data; registerToken(&info); }

		void setValue(const T &value) { m_value = value; }
		T getValue() const { return m_value; }

	protected:
		T m_value;
		_TokenInfo m_token;
	};

	// Specialized type for labels
	class SdlValueLabel : public SdlValueConstant<std::string, SdlValue::CONSTANT_LABEL> {
		// Resolution stage
	public:
		SdlValueLabel(const _TokenInfo &value) : SdlValueConstant(value) { /* void */ }
		~SdlValueLabel() { /* void */ }

	protected:
		virtual void _resolveReferences(SdlCompilationUnit *unit) {
			m_reference = resolveName(m_value);

			if (m_reference == nullptr) {
				unit->addCompilationError(new SdlCompilationError(m_summaryToken,
					SdlErrorCode::UnresolvedReference));
			}
		}
	};

	typedef SdlValueConstant<int, SdlValue::CONSTANT_INT> SdlValueInt;
	typedef SdlValueConstant<SdlNode *, SdlValue::NODE_REF> SdlValueNodeRef;
	typedef SdlValueConstant<std::string, SdlValue::CONSTANT_STRING> SdlValueString;
	typedef SdlValueConstant<double, SdlValue::CONSTANT_FLOAT> SdlValueFloat;
	typedef SdlValueConstant<bool, SdlValue::CONSTANT_BOOL> SdlValueBool;

} /* namespace manta */

#endif /* SDL_VALUE_CONSTANT_H */
