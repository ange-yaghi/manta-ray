#include <sdl_parser_structure.h>

manta::SdlParserStructure::SdlParserStructure() {
	/* void */
}

manta::SdlParserStructure::~SdlParserStructure() {
	/* void */
}

void manta::SdlParserStructure::registerToken(const SdlTokenInfo *tokenInfo) {
	if (tokenInfo != nullptr) m_summaryToken.combine(tokenInfo);
}

void manta::SdlParserStructure::registerComponent(const SdlParserStructure *child) {
	if (child != nullptr) m_summaryToken.combine(child->getSummaryToken());
}
