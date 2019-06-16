#ifndef SDL_NODE_DEFINITION_H
#define SDL_NODE_DEFINITION_H

#include <sdl_parser_structure.h>

#include <sdl_token_info.h>
#include <sdl_structure_list.h>

namespace manta {

	class SdlAttributeDefinitionList;
	class SdlAttributeDefinition;
	class SdlCompilationUnit;

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
		const SdlAttributeDefinitionList *getAttributeDefinitionList() const { return m_attributes; }

		void setScope(SCOPE scope) { m_scope = scope; }
		SCOPE getScope() const { return m_scope; }

		void setScopeToken(const SdlTokenInfo_string &token);
		const SdlTokenInfo *getScopeToken() const { return &m_scopeToken; }

		SdlAttributeDefinition *getAttributeDefinition(const std::string &attributeName) const;

		void setBody(SdlNodeList *body) { m_body = body; registerComponent(body); }
		SdlNodeList *getBody() const { return m_body; }

		virtual SdlParserStructure *resolveName(const std::string &name);

	protected:
		SdlTokenInfo_string m_name;

		bool m_definesBuiltin;
		SdlTokenInfo_string m_builtinName;

		SdlAttributeDefinitionList *m_attributes;
		SdlNodeList *m_body;

		SdlTokenInfo_string m_scopeToken;
		SCOPE m_scope;

		// Resolution stage
	public:
		int countSymbolIncidence(const std::string &name) const;
		SdlParserStructure *resolveLocalName(const std::string &name);

		// Validation stage
	protected:
		virtual void _validate(SdlCompilationUnit *unit);
	};

} /* namespace manta */

#endif /* SDL_NODE_DEFINITION_H */