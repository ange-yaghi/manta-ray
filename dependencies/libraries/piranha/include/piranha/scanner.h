#ifndef PIRANHA_SCANNER_H
#define PIRANHA_SCANNER_H

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <parser.auto.h>
#include "ir_token_info.h"
#include "ir_token_info.h"

namespace piranha {

	class IrScanner : public yyFlexLexer {
	public:
		IrScanner(std::istream *in) : yyFlexLexer(in) {
			m_loc = new piranha::IrParser::location_type();
			m_loc->colStart = m_loc->colEnd = 1;
			m_loc->lineStart = m_loc->lineEnd = 1;
		}

		using FlexLexer::yylex;

		virtual int yylex(piranha::IrParser::semantic_type *lval,
			piranha::IrParser::location_type *location);

	private:
		void comment();
		void count();
		void countChar(char c);
		void step();

		template <typename T>
		void buildValue(const T &data, piranha::IrParser::location_type *location, bool valid=true) {
			typedef T_IrTokenInfo<T> _TokenInfo;

			_TokenInfo info(data, m_loc->lineStart, m_loc->lineEnd, 
				m_loc->colStart, m_loc->colEnd);
			info.valid = valid;
			m_yylval->build<_TokenInfo>(info);
			*location = info;
		}

		piranha::IrParser::semantic_type *m_yylval = nullptr;
		piranha::IrParser::location_type *m_loc = nullptr;
	};

} /* namespace piranha */

#endif /* PIRANHA_SCANNER_H */
