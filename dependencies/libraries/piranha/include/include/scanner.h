#ifndef PIRANHA_SCANNER_H
#define PIRANHA_SCANNER_H

#if !defined(yyFlexLexerOnce)
#ifdef PIRANHA_DIST
#include "../lib/FlexLexer.h"
#else
#include <FlexLexer.h>
#endif /* PIRANHA_DIST */
#endif

#include "../autogen/parser.auto.h"
#include "ir_token_info.h"
#include "ir_token_info.h"

namespace piranha {

    class Scanner : public yyFlexLexer {
    public:
        Scanner(std::istream *in) : yyFlexLexer(in) {
            m_loc = new piranha::Parser::location_type();
            m_loc->colStart = m_loc->colEnd = 1;
            m_loc->lineStart = m_loc->lineEnd = 1;
        }

        using FlexLexer::yylex;

        virtual int yylex(
            piranha::Parser::semantic_type *lval, piranha::Parser::location_type *location);

    private:
        void comment();
        void count();
        void countChar(char c);
        void step();

        template <typename T>
        void buildValue(const T &data, piranha::Parser::location_type *location, bool valid = true) {
            typedef T_IrTokenInfo<T> _TokenInfo;

            _TokenInfo info(data, m_loc->lineStart, m_loc->lineEnd, 
                m_loc->colStart, m_loc->colEnd);
            info.valid = valid;
            m_yylval->build<_TokenInfo>(info);
            *location = info;
        }

        piranha::Parser::semantic_type *m_yylval = nullptr;
        piranha::Parser::location_type *m_loc = nullptr;
    };

} /* namespace piranha */

#endif /* PIRANHA_SCANNER_H */
