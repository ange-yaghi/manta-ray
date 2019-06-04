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
		void registerComponent(const SdlParserStructure *child);

	protected:
		SdlTokenInfo m_summaryToken;
	};

} /* namespace manta */

#endif /* SDL_PARSER_STRUCTURE_H */