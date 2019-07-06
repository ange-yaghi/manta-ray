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
	#include <sdl_visibility.h>
	#include <sdl_unary_operator.h>
	#include <sdl_generic_value.h>

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
%token <manta::SdlTokenInfo_string> AS
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
%token <manta::SdlTokenInfo_string> PUBLIC
%token <manta::SdlTokenInfo_string> PRIVATE
%token <manta::SdlTokenInfo_string> BUILTIN_POINTER
%token <manta::SdlTokenInfo_string> POINTER
%token <manta::SdlTokenInfo_string> NAMESPACE_POINTER
%token <manta::SdlTokenInfo_string> UNRECOGNIZED
%token <manta::SdlTokenInfo_string> OPERATOR
%token <manta::SdlTokenInfo_string> MODULE

%token <manta::SdlTokenInfo_string> '='
%token <manta::SdlTokenInfo_string> '+'
%token <manta::SdlTokenInfo_string> '-'
%token <manta::SdlTokenInfo_string> '/'
%token <manta::SdlTokenInfo_string> '*'
%token <manta::SdlTokenInfo_string> '('
%token <manta::SdlTokenInfo_string> ')'
%token <manta::SdlTokenInfo_string> '{'
%token <manta::SdlTokenInfo_string> '}'
%token <manta::SdlTokenInfo_string> '['
%token <manta::SdlTokenInfo_string> ']'
%token <manta::SdlTokenInfo_string> ':'
%token <manta::SdlTokenInfo_string> ';'
%token <manta::SdlTokenInfo_string> ','
%token <manta::SdlTokenInfo_string> '.'
%token <manta::SdlTokenInfo_string> '^'

%type <manta::SdlTokenInfo_string> standard_operator;
%type <manta::SdlTokenInfo_string> type_name;
%type <manta::SdlTokenInfoSet<std::string, 2>> type_name_namespace;
%type <manta::SdlNode *> node;
%type <manta::SdlNodeList *> node_list;
%type <manta::SdlAttributeList *> attribute_list;
%type <manta::SdlAttributeList *> connection_block;
%type <manta::SdlAttribute *> attribute;
%type <manta::SdlValue *> value;
%type <manta::SdlValue *> port_value;
%type <manta::SdlValue *> atomic_value;
%type <manta::SdlValue *> label_value;
%type <manta::SdlNode *> inline_node;
%type <manta::SdlValue *> constant;
%type <manta::SdlValue *> data_access;
%type <manta::SdlValue *> default_operator;
%type <manta::SdlValue *> mul_exp;
%type <manta::SdlValue *> add_exp;
%type <manta::SdlValue *> primary_exp;
%type <manta::SdlImportStatement *> import_statement;
%type <manta::SdlImportStatement *> import_statement_visibility;
%type <manta::SdlImportStatement *> import_statement_short_name;
%type <manta::SdlTokenInfo_string> string;

%type <manta::SdlNodeDefinition *> node_name;
%type <manta::SdlNodeDefinition *> node_shadow;
%type <manta::SdlNodeDefinition *> node_decorator;
%type <manta::SdlNodeDefinition *> node_definition;
%type <manta::SdlNodeDefinition *> node_port_definitions;
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
  : node 							{ driver.addNode($1); }
  | import_statement_short_name		{ driver.addImportStatement($1); }
  | node_decorator					{ driver.addNodeDefinition($1); }
  | MODULE '{' decorator_list '}'	{ }
  ;

statement_list 
  : statement						{ }
  | statement_list statement		{ }
  ;

import_statement
  : IMPORT string					{ $$ = new SdlImportStatement($2); }
  | IMPORT LABEL					{ 
										$$ = new SdlImportStatement($2); 

										/* The name is a valid label so it can be used as a short name */
										$$->setShortName($2); 
									}
  ;

import_statement_visibility
  : PUBLIC import_statement			{ $$ = $2; $$->setVisibility(SdlVisibility::PUBLIC); }
  | PRIVATE import_statement		{ $$ = $2; $$->setVisibility(SdlVisibility::PRIVATE); }
  | import_statement				{ $$ = $1; $$->setVisibility(SdlVisibility::DEFAULT); }
  ;

import_statement_short_name
  : import_statement_visibility AS LABEL	{ $$ = $1; $$->setShortName($3); }
  | import_statement_visibility				{ $$ = $1; }
  ;

type_name
  : LABEL							{ $$ = $1; }
  | OPERATOR standard_operator		{
										SdlTokenInfo_string info = $1;
										info.combine(&$2);
										info.data = std::string("operator") + $2.data;
										$$ = info;										
									}
  ;

type_name_namespace
  : type_name							{ 
											SdlTokenInfoSet<std::string, 2> set; 
											set.data[1] = $1; 
											$$ = set; 
										}
  | LABEL NAMESPACE_POINTER type_name	{ 
											SdlTokenInfoSet<std::string, 2> set; 
											set.data[0] = $1; 
											set.data[1] = $3; 
											$$ = set; 
										}
  | NAMESPACE_POINTER type_name			{ 
											SdlTokenInfoSet<std::string, 2> set; 
											set.data[0].specified = true; 
											set.data[1] = $2; 
											$$ = set; 
										}
  ;

node
  : type_name_namespace LABEL connection_block			{ $$ = new SdlNode($1.data[1], $2, $3, $1.data[0]); }
  | type_name_namespace connection_block				{
															SdlTokenInfo_string name;
															name.specified = false;
															name.data = "";

															$$ = new SdlNode($1.data[1], name, $2, $1.data[0]);
														}
  ;

node_list
  : node											{
															$$ = new SdlNodeList();
															$$->add($1);
														}
  | node_list node									{ 
															$$ = $1;
															$1->add($2);  
														}
  ;

standard_operator
  : '-'													{ $$ = $1; }
  | '+'													{ $$ = $1; }
  | '/'													{ $$ = $1; }
  | '*'													{ $$ = $1; }
  ;

node_name
  : NODE LABEL											{ $$ = new SdlNodeDefinition($2); }
  | NODE OPERATOR standard_operator						{
															SdlTokenInfo_string info = $2;
															info.combine(&$3);
															info.data = std::string("operator") + $3.data;
															$$ = new SdlNodeDefinition(info);
														}
  ;

node_shadow
  : node_name BUILTIN_POINTER LABEL						{ $$ = $1; $$->setBuiltinName($3); $$->setDefinesBuiltin(true); }
  | node_name											{ $$ = $1; $$->setDefinesBuiltin(false); }
  ;

node_port_definitions
  : node_shadow '{' port_definitions					{ $$ = $1; $$->setAttributeDefinitionList($3); }
  | node_shadow '{'										{ 
															$$ = $1; 
															$$->setAttributeDefinitionList(new SdlAttributeDefinitionList()); 
														}
  ;

node_definition
  : node_port_definitions '}'							{ $$ = $1; $$->setBody(nullptr); }
  | node_port_definitions node_list '}'					{ $$ = $1; $$->setBody($2); }
  | error '{' port_definitions '}'						{ $$ = nullptr; yyerrok; }
  | error '{' port_definitions node_list '}'			{ $$ = nullptr; yyerrok; }
  ;

specific_node_definition
  : node_definition										{ $$ = $1; }
  | PRIVATE node_definition								{ 
															if ($2 != nullptr) {
																$$ = $2; 
																$$->setVisibility(SdlVisibility::PRIVATE); 
																$$->setScopeToken($1);
															}
															else $$ = nullptr;
														}
  | PUBLIC node_definition								{ 
															if ($2 != nullptr) {
																$$ = $2;
																$$->setVisibility(SdlVisibility::PUBLIC);
																$$->setScopeToken($1);
															}
															else $$ = nullptr;
														}
  ;

node_decorator
  : decorator_list specific_node_definition				{ $$ = $2; }
  | specific_node_definition							{ $$ = $1; }
  ;

port_definitions
  : documented_port_definition							{ 
															$$ = new SdlAttributeDefinitionList(); 
															$$->addDefinition($1); 
														}
  | port_definitions documented_port_definition			{ $$ = $1; $$->addDefinition($2); }
  ;

port_declaration
  : INPUT LABEL													{ $$ = new SdlAttributeDefinition($1, $2, SdlAttributeDefinition::INPUT); }
  | OUTPUT LABEL												{ $$ = new SdlAttributeDefinition($1, $2, SdlAttributeDefinition::OUTPUT); }
  ;

port_status
  : DEFAULT port_declaration			{ $$ = $2; $$->setDefault(true); $$->setDefaultToken($1); }
  | port_declaration					{ $$ = $1; $$->setDefault(false); }
  ;

port_value
  : '[' type_name_namespace ']'			{ $$ = new SdlGenericValue($2); }
  | value								{ $$ = $1; }
  ;

port_connection
  : port_status ':' port_value			{ $$ = $1; $$->setDefaultValue($3); }
  | port_status							{ $$ = $1; $$->setDefaultValue(nullptr); }
  ;

documented_port_definition
  : decorator_list port_connection ';'	{ $$ = $2; }
  | port_connection ';'					{ $$ = $1; }
  | error ';'							{ $$ = nullptr; }
  ;

inline_node
  : type_name_namespace connection_block	{ $$ = new SdlNode($1.data[1], $2, $1.data[0]); }
  ;

connection_block 
  : '(' ')'							{ 
										$$ = new SdlAttributeList(); 
										$$->registerToken(&$1); 
										$$->registerToken(&$2); 
									}
  | '(' attribute_list ')'			{ 
										$$ = $2; 
										$$->registerToken(&$1); 
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
  | '(' error ')'					{ $$ = nullptr; yyerrok; }
  ;

default_operator
  : primary_exp						{ $$ = $1; }
  | default_operator '^'			{ $$ = new SdlUnaryOperator(SdlUnaryOperator::DEFAULT, $1); }
  ;

data_access
  : default_operator				{ $$ = $1; }
  | data_access '.' label_value		{ 
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::DOT, $1, $3));
									}
  | data_access POINTER label_value { 
										$$ = static_cast<SdlValue *>(
											new SdlBinaryOperator(SdlBinaryOperator::POINTER, $1, $3));
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
		err = new SdlCompilationError(l, SdlErrorCode::UnexpectedToken);
	}
	else {
		err = new SdlCompilationError(l, SdlErrorCode::UnidentifiedToken);
	}

	driver.addCompilationError(err);
}
