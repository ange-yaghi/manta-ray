%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {manta}
%define api.parser.class {SdlParser}

%code requires {
	namespace manta {
		class SdlDriver;
		class SdlScanner;
	}

	#include <sdl_driver.h>
	#include <sdl_node.h>
	#include <sdl_attribute_list.h>
	#include <sdl_attribute.h>
	#include <sdl_value.h>
	#include <sdl_value_label.h>
	#include <sdl_value_node_ref.h>

	#include <string>

	#ifndef YY_NULLPTR
	#if defined __cplusplus && 201103L <= __cplusplus
	#define YY_NULLPTR nullptr
	#else
	#define YY_NULLPTR 0
	#endif
	#endif

	/* Remove annoying compiler warnings */
    #ifdef _MSC_VER
    /* warning C4065: switch statement contains 'default' but no 'case' labels */
    #pragma warning (disable : 4065)
    #endif
}

%type <manta::SdlNode *> node;
%type <manta::SdlDriver *> node_list;
%type <manta::SdlAttributeList *> attribute_list;
%type <manta::SdlAttributeList *> connection_block;
%type <manta::SdlAttribute *> attribute;
%type <manta::SdlValue *> value;
%type <manta::SdlNode *> inline_node;

%parse-param {SdlScanner &scanner}
%parse-param {SdlDriver &driver}

%code {
  #include <iostream>
  #include <cstdlib>
  #include <fstream>

  #include <sdl_driver.h>
  #include <sdl_scanner.h>

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token END 0
%token UPPER
%token LOWER
%token NEWLINE
%token CHAR
%token <std::string> LABEL

%locations

%%

sdl : END | node_list END;

node_list 
  : node			{ $$ = &driver; $$->addNode($1); }
  | node_list node	{ $1->addNode($2); $$ = $1; }
  ;

node
  : LABEL LABEL connection_block	{ $$ = driver.createNode($1, $2, $3); }
  ;

inline_node
  : LABEL connection_block		{ $$ = driver.createNode($1, "", $2); }
  ;

connection_block 
  : '(' ')'									{ $$ = driver.createAttributeList(); }
  | '(' attribute_list ')'					{ $$ = $2; }
  ;

attribute_list
  : attribute					    { $$ = driver.createAttributeList(); $$->addAttribute($1); }
  | attribute_list ',' attribute	{ $1->addAttribute($3); $$ = $1; }
  ;

attribute
  : LABEL ':' value					{ $$ = driver.createAttribute($1, $3); }
  | value							{ $$ = driver.createAttribute("", $1); }
  ;

value
  : LABEL							{ $$ = static_cast<SdlValue *>(new SdlValueLabel($1)); }
  | inline_node						{ $$ = static_cast<SdlValue *>(new SdlValueNodeRef($1)); }
  ;

%%

void manta::SdlParser::error(const location_type &l, const std::string &err_message) {
  std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
