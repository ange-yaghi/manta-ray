#ifndef SDL_PARSER_STRUCTURE_H
#define SDL_PARSER_STRUCTURE_H

#include <sdl_token_info.h>

#include <vector>

namespace manta {

	class SdlValue;
	class SdlCompilationUnit;

	class SdlParserStructure {
	public:
		SdlParserStructure();
		~SdlParserStructure();

		const SdlTokenInfo *getSummaryToken() const { return &m_summaryToken; }
		void registerToken(const SdlTokenInfo *tokenInfo);

		void registerComponent(SdlParserStructure *child);
		int getComponentCount() const { return (int)m_components.size(); }

		//virtual SdlParserStructure *getPublicAttribute(const std::string &name, bool *failed = nullptr) const { return nullptr; }

		void setParentScope(SdlParserStructure *parentScope) { m_parentScope = parentScope; }
		SdlParserStructure *resolveName(const std::string &name) const;
		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;

		bool getDefinitionsResolved() const { return m_definitionsResolved; }
		bool getReferencesResolved() const { return m_referencesResolved; }
		bool isExpanded() const { return m_isExpanded; }
		bool isValidated() const { return m_validated; }
		SdlParserStructure *getImmediateReference() const { return m_reference; }
		SdlParserStructure *getReference();
		SdlParserStructure *getExpansion() const { return m_expansion; }

		virtual SdlValue *getAsValue() { return nullptr; }

		bool allowsExternalAccess() const { return m_externalAccess; }
		void setExternalAccess(bool externalAccess) { m_externalAccess = externalAccess; }

	public:
		// Compilation stages
		void expand(SdlCompilationUnit *unit);
		void resolveDefinitions(SdlCompilationUnit *unit);
		void resolveReferences(SdlCompilationUnit *unit);
		void validate(SdlCompilationUnit *unit);

	protected:
		virtual void _expand(SdlCompilationUnit *unit);
		virtual void _resolveDefinitions(SdlCompilationUnit *unit);
		virtual void _resolveReferences(SdlCompilationUnit *unit);
		virtual void _validate(SdlCompilationUnit *unit);

	protected:
		SdlParserStructure *m_parentScope;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_reference;
		SdlParserStructure *m_expansion;

		std::vector<SdlParserStructure *> m_components;

	protected:
		// Compilation flags
		bool m_isExpanded;
		bool m_definitionsResolved;
		bool m_referencesResolved;
		bool m_validated;

		bool m_resolveReferencesChildren;
		bool m_externalAccess;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */
