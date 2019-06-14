#ifndef SDL_PARSER_STRUCTURE_H
#define SDL_PARSER_STRUCTURE_H

#include <sdl_token_info.h>

#include <vector>

namespace manta {

	class SdlValue;

	class SdlParserStructure {
	public:
		SdlParserStructure();
		~SdlParserStructure();

		const SdlTokenInfo *getSummaryToken() const { return &m_summaryToken; }
		void registerToken(const SdlTokenInfo *tokenInfo);
		void registerComponent(SdlParserStructure *child);

		virtual SdlParserStructure *getPublicAttribute(const std::string &name) { return nullptr; }

		void setParentScope(SdlParserStructure *parentScope) { m_parentScope = parentScope; }
		SdlParserStructure *resolveName(const std::string &name) const;

		bool isResolved() const { return m_isResolved; }
		bool isExpanded() const { return m_isExpanded; }
		SdlParserStructure *getReference() const { return m_reference; }
		SdlParserStructure *getExpansion() const { return m_expansion; }

		virtual SdlValue *getAsValue() { return nullptr; }

	public:
		// Compilation stages
		void expand();
		void resolve();

	protected:
		virtual void _expand();
		virtual void _resolve();

	protected:
		SdlParserStructure *resolveLocalName() const;

		SdlParserStructure *m_parentScope;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_reference;
		SdlParserStructure *m_expansion;

	protected:
		// Compilation flags
		bool m_isExpanded;
		bool m_isResolved;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */
