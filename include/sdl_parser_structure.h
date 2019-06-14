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
		virtual SdlParserStructure *getDefaultInterface() { return this; }

		void setParentScope(SdlParserStructure *parentScope) { m_parentScope = parentScope; }
		SdlParserStructure *resolveName(const std::string &name) const;

		bool isResolved() const { return m_isResolved; }
		SdlParserStructure *getReference() { return m_reference; }
		virtual void resolveReferences();

	protected:
		SdlParserStructure *resolveLocalName() const;

		SdlParserStructure *m_parentScope;
		SdlTokenInfo m_summaryToken;

		SdlParserStructure *m_reference;
		bool m_isResolved;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */