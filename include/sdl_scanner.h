#ifndef SDL_SCANNER_H
#define SDL_SCANNER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <sdl_parser.auto.h>


namespace manta {

	class SdlScanner : public yyFlexLexer {
	public:
		SdlScanner(std::istream *in) : yyFlexLexer(in) {
			m_loc = new manta::SdlParser::location_type();
		}

		using FlexLexer::yylex;

		virtual int yylex(manta::SdlParser::semantic_type * lval,
			manta::SdlParser::location_type *location);

	private:
		manta::SdlParser::semantic_type *m_yylval = nullptr;
		manta::SdlParser::location_type *m_loc = nullptr;
	};

} /* namespace manta */

#endif /* SDL_SCANNER_H */
