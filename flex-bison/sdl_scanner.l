%{
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include <sdl_scanner.h>
#undef  YY_DECL
#define YY_DECL int manta::SdlScanner::yylex( manta::SdlParser::semantic_type *const lval, manta::SdlParser::location_type *location)

/* using "token" to make the returns for the tokens shorter to type */
using token = manta::SdlParser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION m_loc->step(); m_loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="manta::SdlScanner"
%option noyywrap
%option c++

D				[0-9]
L				[a-zA-Z_]

%%
%{          /** Code executed at the beginning of yylex **/
            m_yylval = lval;
%}


{L}({L}|{D})*	{
					m_yylval->build<std::string>(yytext);
					return (token::LABEL);
				}

","				{ return (','); }
"("				{ return ('('); }
")"				{ return (')'); }
":"				{ return (':'); }
";"				{ return (';'); }
","				{ return (','); }
[ \t\v\n\f]		{ }
.				{ }
%%
