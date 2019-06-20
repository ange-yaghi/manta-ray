#ifndef SDL_PARSER_STRUCTURE_H
#define SDL_PARSER_STRUCTURE_H

#include <sdl_token_info.h>

#include <vector>

namespace manta {

	class SdlValue;
	class SdlCompilationUnit;
	class SdlCompilationError;

	class SdlParserStructure {
	public:
		SdlParserStructure();
		~SdlParserStructure();

		const SdlTokenInfo *getSummaryToken() const { return &m_summaryToken; }
		void registerToken(const SdlTokenInfo *tokenInfo);

		void registerComponent(SdlParserStructure *child);
		int getComponentCount() const { return (int)m_components.size(); }

		void setParentScope(SdlParserStructure *parentScope) { m_parentScope = parentScope; }
		virtual SdlParserStructure *resolveName(const std::string &name) const;
		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;

		bool getDefinitionsResolved() const { return m_definitionsResolved; }
		bool isExpanded() const { return m_isExpanded; }
		bool isValidated() const { return m_validated; }
		virtual SdlParserStructure *getImmediateReference(SdlCompilationError **err = nullptr) { return nullptr; }
		SdlParserStructure *getReference(SdlCompilationError **err = nullptr);
		SdlParserStructure *getExpansion() const { return m_expansion; }

		virtual SdlValue *getAsValue() { return nullptr; }

		bool allowsExternalAccess() const { return m_externalAccess; }
		void setExternalAccess(bool externalAccess) { m_externalAccess = externalAccess; }

		void setCheckReferences(bool check) { m_checkReferences = check; }
		bool getCheckReferences() const { return m_checkReferences; }

	public:
		// Compilation stages
		void expand(SdlCompilationUnit *unit);
		void resolveDefinitions(SdlCompilationUnit *unit);
		void checkReferences(SdlCompilationUnit *unit);
		void validate(SdlCompilationUnit *unit);

	protected:
		virtual void _expand(SdlCompilationUnit *unit);
		virtual void _resolveDefinitions(SdlCompilationUnit *unit);
		virtual void _validate(SdlCompilationUnit *unit);

	protected:
		SdlParserStructure *m_parentScope;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_expansion;

		std::vector<SdlParserStructure *> m_components;

	protected:
		// Compilation flags
		bool m_isExpanded;
		bool m_definitionsResolved;
		bool m_validated;

		bool m_checkReferences;

		bool m_externalAccess;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */
