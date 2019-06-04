#ifndef SDL_NODE_DEFINITION_H
#define SDL_NODE_DEFINITION_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>

namespace manta {

	class SdlAttributeDefinitionList;

	class SdlNodeDefinition : public SdlParserStructure {
	public:
		enum SCOPE {
			LOCAL,
			EXPORT,
			UNSPECIFIED
		};

	public:
		SdlNodeDefinition(const SdlTokenInfo_string &name);
		virtual ~SdlNodeDefinition();

		const SdlTokenInfo *getBuiltinNameToken() const { return m_definesBuiltin ? &m_builtinName : nullptr; }
		std::string getBuiltinName() const { return m_builtinName.data; }
		void setBuiltinName(const SdlTokenInfo_string &builtin);

		void setDefinesBuiltin(bool defines) { m_definesBuiltin = defines; }
		bool isBuiltin() const { return m_definesBuiltin; }

		const SdlTokenInfo *getNameToken() const { return &m_name; }
		std::string getName() const { return m_name.data; }

		void setAttributeDefinitionList(SdlAttributeDefinitionList *definitions);
		const SdlAttributeDefinitionList *getAttributionDefinitionList() const { return m_attributes; }

		void setScope(SCOPE scope) { m_scope = scope; }
		SCOPE getScope() const { return m_scope; }

		void setScopeToken(const SdlTokenInfo_string &token);
		const SdlTokenInfo *getScopeToken() const { return &m_scopeToken; }

	protected:
		SdlTokenInfo_string m_name;

		bool m_definesBuiltin;
		SdlTokenInfo_string m_builtinName;

		SdlAttributeDefinitionList *m_attributes;

		SdlTokenInfo_string m_scopeToken;
		SCOPE m_scope;
	};

} /* namespace manta */

#endif /* SDL_NODE_DEFINITION_H */