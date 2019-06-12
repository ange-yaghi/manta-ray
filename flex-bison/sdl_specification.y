%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace {manta}
%define api.parser.class {SdlParser}

%code requires {
	namespace manta {
		class SdlCompilationUnit;
		class SdlScanner;
	}

	#include <sdl_compilation_unit.h>
	#include <sdl_node.h>
	#include <sdl_attribute_list.h>
	#include <sdl_attribute.h>
	#include <sdl_value.h>
	#include <sdl_value_constant.h>
	#include <sdl_binary_operator.h>
	#include <sdl_import_statement.h>
	#include <sdl_token_info.h>
	#include <sdl_node_definition.h>
	#include <sdl_attribute_definition.h>
	#include <sdl_attribute_definition_list.h>
	#include <sdl_compilation_error.h>
	#include <sdl_structure_list.h>

	#include <string>

	#ifndef YY_NULLPTR
	#if defined __cplusplus && 201103L <= __cplusplus
	#define YY_NULLPTR nullptr
	#else
	#define YY_NULLPTR 0
	#endif
	#endif

	# define YYLLOC_DEFAULT(Cur, Rhs, N)						\
	do															\
		if (N) {												\
			(Cur).combine(&YYRHSLOC(Rhs, 1));					\
			(Cur).combine(&YYRHSLOC(Rhs, N));					\
		}														\
		else {                                                  \
			(Cur).combine(&YYRHSLOC(Rhs, 0));					\
		}														\
	while (0)

	/* Remove annoying compiler warnings */
    #ifdef _MSC_VER
    /* warning C4065: switch statement contains 'default' but no 'case' labels */
    #pragma warning (disable: 4065)
    #endif
}

%parse-param {SdlScanner &scanner}
%parse-param {SdlCompilationUnit &driver}

%code {
  #include <iostream>
  #include <cstdlib>
  #include <fstream>

  #include <sdl_compilation_unit.h>
  #include <sdl_scanner.h>

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define api.location.type {manta::SdlTokenInfo}
%define parse.assert

%token END 0
%token <manta::SdlTokenInfo_string> CHAR
%token <manta::SdlTokenInfo_string> IMPORT
%token <manta::SdlTokenInfo_string> NODE
%token <manta::SdlTokenInfo_string> DEFAULT
%token <manta::SdlTokenInfo_string> INPUT
%token <manta::SdlTokenInfo_string> OUTPUT
%token <manta::SdlTokenInfo_string> LABEL
%token <manta::SdlTokenInfo_int>	INT
%token <manta::SdlTokenInfo_float>	FLOAT
%token <manta::SdlTokenInfo_bool>	BOOL
%token <manta::SdlTokenInfo_string> STRING
%token <manta::SdlTokenInfo_string> DECORATOR
%token <manta::SdlTokenInfo_string> LOCAL
%token <manta::SdlTokenInfo_string> EXPORT
%token <manta::SdlTokenInfo_string> POINTER
%token <manta::SdlTokenInfo_string> UNRECOGNIZED

%token <manta::SdlTokenInfo_string> '='
%token <manta::SdlTokenInfo_string> '+'
%token <manta::SdlTokenInfo_string> '-'
%token <manta::SdlTokenInfo_string> '/'
%token <manta::SdlTokenInfo_string> '*'
%token <manta::SdlTokenInfo_string> '('
%token <manta::SdlTokenInfo_string> ')'
%token <manta::SdlTokenInfo_string> '{'
%token <manta::SdlTokenInfo_string> '}'
%token <manta::SdlTokenInfo_string> ':'
%token <manta::SdlTokenInfo_string> ';'
%token <manta::SdlTokenInfo_string> ','
%token <manta::SdlTokenInfo_string> '.'

%type <manta::SdlNode *> node;
%type <manta::SdlNodeList *> node_list;
%type <manta::SdlAttributeList *> attribute_list;
%type <manta::SdlAttributeList *> connection_block;
%type <manta::SdlAttribute *> attribute;
%type <manta::SdlValue *> value;
%type <manta::SdlValue *> atomic_value;
%type <manta::SdlValue *> label_value;
%type <manta::SdlNode *> inline_node;
%type <manta::SdlValue *> constant;
%type <manta::SdlValue *> data_access;
%type <manta::SdlValue *> mul_exp;
%type <manta::SdlValue *> add_exp;
%type <manta::SdlValue *> primary_exp;
%type <manta::SdlImportStatement *> import_statement;
%type <manta::SdlTokenInfo_string> string;

%type <manta::SdlNodeDefinition *> node_name;
%type <manta::SdlNodeDefinition *> node_shadow;
%type <manta::SdlNodeDefinition *> node_decorator;
%type <manta::SdlNodeDefinition *> node_definition;
%type <manta::SdlNodeDefinition *> specific_node_definition;

%type <manta::SdlAttributeDefinition *> port_declaration;
%type <manta::SdlAttributeDefinition *> port_status;
%type <manta::SdlAttributeDefinition *> port_connection;
%type <manta::SdlAttributeDefinition *> documented_port_definition;

%type <manta::SdlAttributeDefinitionList *> port_definitions;

%locations

%%

sdl 
  : END 
  | decorator_list statement_list END
  | statement_list END
  ;

decorator
  : DECORATOR LABEL '=' string	{}
  ;

decorator_list
  : decorator					{}
  | decorator_list decorator	{}
  ;

statement
  : node ';'						{ driver.addNode($1); }
  | import_statement				{ driver.addImportStatement($1); }
  | specific_node_definition ';'	{ }
  ;

statement_list 
  : statement						{ }
  | statement_list statement		{ }
  ;

import_statement
  : IMPORT string					{ $$ = new SdlImportStatement($2); }
  | IMPORT LABEL					{ $$ = new SdlImportStatement($2); }
  ;

node
  : LABEL LABEL connection_block						{ $$ = new SdlNode($1, $2, $3); }
  ;

node_list
  : node ';'											{
															$$ = new SdlNodeList();
															$$->add($1);
														}
  | node_list node ';'									{ $1->add($2); }
  ;

node_name
  : NODE LABEL											{ 
															$$ = new SdlNodeDefinition($2); 
															driver.addNodeDefinition($$); 
														}
  ;

node_shadow
  : node_name POINTER LABEL	':'							{ $$ = $1; $$->setBuiltinName($3); $$->setDefinesBuiltin(true); }
  | node_name ':'										{ $$ = $1; $$->setDefinesBuiltin(false); }
  ;

node_decorator
  : node_shadow decorator_list '{' port_definitions		{ $$ = $1; $$->setAttributeDefinitionList($4); }
  | node_shadow '{' port_definitions					{ $$ = $1; $$->setAttributeDefinitionList($3); }
  ;

node_definition
  : node_decorator '}'									{ $$ = $1; $$->setBody(nullptr); }
  | node_decorator node_list '}'						{ $$ = $1; $$->setBody($2); }
  ;

specific_node_definition
  : node_definition										{ $$ = $1; }
  | LOCAL node_definition								{ 
															$$ = $2; 
															$$->setScope(SdlNodeDefinition::LOCAL); 
															$$->setScopeToken($1); 
														}
  | EXPORT node_definition								{ 
															$$ = $2;
															$$->setScope(SdlNodeDefinition::EXPORT);
															$$->setScopeToken($1);
														}
  ;

port_definitions
  : documented_port_definition	';'						{ 
															$$ = new SdlAttributeDefinitionList(); 
															$$->addDefinition($1); 
														}
  | port_definitions documented_port_definition	';'		{ $$ = $1; $$->addDefinition($2); }
  ;

port_declaration
  : INPUT LABEL							{ $$ = new SdlAttributeDefinition($1, $2, SdlAttributeDefinition::INPUT); }
  | OUTPUT LABEL						{ $$ = new SdlAttributeDefinition($1, $2, SdlAttributeDefinition::OUTPUT); }
  ;

port_status
  : DEFAULT port_declaration			{ $$ = $2; $$->setDefault(true); $$->setDefaultToken($1); }
  | port_declaration					{ $$ = $1; $$->setDefault(false); }
  ;

port_connection
  : port_status ':' value				{ $$ = $1; $$->setDefaultValue($3); }
  | port_status							{ $$ = $1; $$->setDefaultValue(nullptr); }
  ;

documented_port_definition
  : decorator_list port_connection		{ $$ = $2; }
  | port_connection						{ $$ = $1; }
  ;

inline_node
  : LABEL connection_block			{ $$ = new SdlNode($1, $2); }
  ;

connection_block 
  : '(' ')'							{ 
										$$ = new SdlAttributeList(); 
										$$->registerToken(&$1); 
										$$->registerToken(&$2); 
									}
  | '(' attribute_list ')'			{ 
										$$ = $2; $$->registerToken(&$1); 
										$$->registerToken(&$3); 
									}
  | '(' error ')'					{ yyerrok; }
  ;

attribute_list
  : attribute					    { 
										$$ = new SdlAttributeList();
										$$->addAttribute($1); 
									}
  | attribute_list ',' attribute	{
										$1->addAttribute($3); 
										$$ = $1; 
									}
  | error ',' attribute				{
										$$ = new SdlAttributeList();
										$$->addAttribute($3);
									}
  ;

attribute
  : LABEL ':' value					{ $$ = new SdlAttribute($1, $3); }
  | value							{ $$ = new SdlAttribute($1); }
  ;

label_value
  : LABEL							{ $$ = static_cast<SdlValue *>(new SdlValueLabel($1)); }
  ;

value
  : add_exp							{ $$ = $1; }
  ;

string
  : STRING							{ $$ = $1; @$ = $1; }
  | string STRING					{ 
										$$ = SdlTokenInfo_string();
										$$.data = $1.data + $2.data;
										$$.combine(&$1);
										$$.combine(&$2);
									}
  ;

constant
  : INT								{ $$ = static_cast<SdlValue *>(new SdlValueInt($1)); @$ = $1; }
  | string							{ $$ = static_cast<SdlValue *>(new SdlValueString($1)); }
  | FLOAT							{ $$ = static_cast<SdlValue *>(new SdlValueFloat($1)); @$ = $1; }
  | BOOL							{ $$ = static_cast<SdlValue *>(new SdlValueBool($1)); @$ = $1; }
  ;

atomic_value
  : label_value						{ $$ = $1; }
  | inline_node						{ $$ = static_cast<SdlValue *>(new SdlValueNodeRef($1)); }
  | constant						{ $$ = $1; }
  ;

primary_exp
  : atomic_value					{ $$ = $1; }
  | '(' value ')'					{ $$ = $2; $$->registerToken(&$1); $$->registerToken(&$3); }
  | '(' error ')'					{ yyerrok; }
  ;

data_access
  : primary_exp						{ $$ = $1; }
  | data_access '.' label_value		{ 
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::DOT, $1, $3));
									}
  ;

mul_exp
  : data_access						{ $$ = $1; }
  | mul_exp '*' data_access			{
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::MUL, $1, $3));
									}
  | mul_exp '/' data_access			{
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::DIV, $1, $3));
									}
  ;

add_exp
  : mul_exp							{ $$ = $1; }
  | add_exp '+' mul_exp				{
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::ADD, $1, $3));
									}
  | add_exp '-' mul_exp				{
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::SUB, $1, $3));
									}
  ;
%%

void manta::SdlParser::error(const SdlTokenInfo &l, const std::string &err_message) {
	SdlCompilationError *err;
	
	if (l.valid) {
		err = new SdlCompilationError(l, {"P", "0001", "Unexpected token"});
	}
	else {
		err = new SdlCompilationError(l, {"S", "0001", "Unidentified token"});
	}

	driver.addCompilationError(err);
}
