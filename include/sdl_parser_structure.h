#ifndef SDL_PARSER_STRUCTURE_H
#define SDL_PARSER_STRUCTURE_H

#include <sdl_token_info.h>
#include <sdl_visibility.h>

#include <vector>

namespace manta {

	class SdlValue;
	class SdlCompilationUnit;
	class SdlCompilationError;
	class SdlNode;
	class Node;

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
		virtual SdlParserStructure *getImmediateReference(SdlParserStructure *inputContext = nullptr, 
			SdlCompilationError **err = nullptr) { return nullptr; }
		SdlParserStructure *getReference(SdlParserStructure *inputContext = nullptr, 
			SdlCompilationError **err = nullptr);
		SdlParserStructure *getExpansion() const { return m_expansion; }

		virtual SdlValue *getAsValue() { return nullptr; }

		bool allowsExternalAccess() const;

		void setVisibility(SdlVisibility visibility) { m_visibility = visibility; }
		SdlVisibility getVisibility() const { return m_visibility; }

		void setDefaultVisibility(SdlVisibility visibility) { m_defaultVisibility = visibility; }
		SdlVisibility getDefaultVisibility() const { return m_defaultVisibility; }

		void setCheckReferences(bool check) { m_checkReferences = check; }
		bool getCheckReferences() const { return m_checkReferences; }

		virtual bool isInputPoint() const { return false; }

		void setParentUnit(SdlCompilationUnit *unit) { m_parentUnit = unit; }
		SdlCompilationUnit *getParentUnit() const;

		virtual SdlNode *getAsNode() { return nullptr; }

	public:
		// Compilation stages
		void expand();
		void resolveDefinitions();
		void checkReferences(SdlParserStructure *inputContext = nullptr);
		void validate();

	protected:
		virtual void _expand();
		virtual void _resolveDefinitions();
		virtual void _validate();
		virtual void _checkInstantiation();

	protected:
		SdlParserStructure *m_parentScope;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_expansion;

		std::vector<SdlParserStructure *> m_components;

		SdlCompilationUnit *m_parentUnit;

	protected:
		// Visibility
		SdlVisibility m_defaultVisibility;
		SdlVisibility m_visibility;

		// Compilation flags
		bool m_isExpanded;
		bool m_definitionsResolved;
		bool m_validated;

		bool m_checkReferences;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */
