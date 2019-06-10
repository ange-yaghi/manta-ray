#include <pch.h>

#include <sdl_compilation_unit.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_value_constant.h>
#include <sdl_binary_operator.h>
#include <sdl_import_statement.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_definition.h>
#include <sdl_attribute_definition_list.h>
#include <sdl_compilation_error.h>
#include <sdl_error_list.h>
#include <sdl_compiler.h>

#include "utilities.h"

using namespace manta;

#define CHECK_SDL_POS(parserStructure, _colStart, _colEnd, _lineStart, _lineEnd)	\
	EXPECT_EQ((parserStructure)->getSummaryToken()->colStart,	(_colStart));		\
	EXPECT_EQ((parserStructure)->getSummaryToken()->colEnd,		(_colEnd));			\
	EXPECT_EQ((parserStructure)->getSummaryToken()->lineStart,	(_lineStart));		\
	EXPECT_EQ((parserStructure)->getSummaryToken()->lineEnd,	(_lineEnd));

#define CHECK_ERROR_CODE(error, _stage, _code)				\
	EXPECT_EQ((error)->getErrorCode().stage, (_stage));	\
	EXPECT_EQ((error)->getErrorCode().code, (_code));
	

TEST(SdlTests, SdlSanityCheck) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_empty_node.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");
	EXPECT_EQ(attributes->getAttributeCount(), 0);
	EXPECT_EQ(parser.getNodeCount(), 1);

	CHECK_SDL_POS(node, 1, 23, 1, 1);
	CHECK_SDL_POS(attributes, 21, 23, 1, 1);
}

TEST(SdlTests, SdlSingleAttribute) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_single_attrib.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute = attributes->getAttribute(0);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute->getName(), "test");
	EXPECT_EQ(attribute->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute->getValue())->getValue(), "test");
	EXPECT_EQ(attributes->getAttributeCount(), 1);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlTwoAttributes) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_two_attribs.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute0->getName(), "test1");
	EXPECT_EQ(attribute0->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute0->getValue())->getValue(), "test");

	EXPECT_EQ(attribute1->getName(), "test2");
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute1->getValue())->getValue(), "test");

	EXPECT_EQ(attributes->getAttributeCount(), 2);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlTwoNodes) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "two_nodes.mr");

	const SdlNode *node1 = parser.getNode(0);
	const SdlAttributeList *attributes1 = node1->getAttributes();

	const SdlNode *node2 = parser.getNode(1);
	const SdlAttributeList *attributes2 = node2->getAttributes();

	EXPECT_EQ(node1->getName(), "testImage");
	EXPECT_EQ(node1->getType(), "FileImage");
	EXPECT_EQ(attributes1->getAttributeCount(), 2);

	EXPECT_EQ(node2->getName(), "testImage2");
	EXPECT_EQ(node2->getType(), "FileImage");
	EXPECT_EQ(attributes2->getAttributeCount(), 2);

	EXPECT_EQ(parser.getNodeCount(), 2);
}

TEST(SdlTests, SdlInlineNode) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_inline_node.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute0->getName(), "test1");
	EXPECT_EQ(attribute0->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute0->getValue())->getValue(), "test");

	EXPECT_EQ(attribute1->getName(), "test2");
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::NODE_REF);

	SdlNode *inlineNode = ((SdlValueNodeRef *)attribute1->getValue())->getValue();
	EXPECT_EQ(inlineNode->getType(), "InlineNode");
	EXPECT_EQ(inlineNode->getName(), "");
	EXPECT_EQ(inlineNode->getAttributes()->getAttributeCount(), 1);

	SdlNode *inlineNode2 = ((SdlValueNodeRef *)attributes->getAttribute(2)->getValue())->getValue();
	EXPECT_EQ(inlineNode2->getType(), "InlineNode2");
	EXPECT_EQ(inlineNode2->getName(), "");
	EXPECT_EQ(inlineNode2->getAttributes()->getAttributeCount(), 1);

	EXPECT_EQ(attributes->getAttributeCount(), 3);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlSimpleIntTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_single_int.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute0->getName(), "test_dec");
	EXPECT_EQ(attribute0->getValue()->getType(), SdlValue::CONSTANT_INT);
	EXPECT_EQ(((SdlValueInt *)attribute0->getValue())->getValue(), 10);

	EXPECT_EQ(attribute1->getName(), "test_hex_1");
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::CONSTANT_INT);
	EXPECT_EQ(((SdlValueInt *)attribute1->getValue())->getValue(), 16);

	EXPECT_EQ(attributes->getAttributeCount(), 2);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlStringSanityCheck) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_string_attrib.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute = attributes->getAttribute(0);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute->getName(), "test");
	EXPECT_EQ(attribute->getValue()->getType(), SdlValue::CONSTANT_STRING);
	EXPECT_EQ(((SdlValueString *)attribute->getValue())->getValue(), "test");
	EXPECT_EQ(attributes->getAttributeCount(), 1);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlSingleNodeDataAccess) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_data_access.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	const SdlValue *v0 = attribute0->getValue();
	const SdlValue *v1 = attribute1->getValue();

	EXPECT_EQ(v0->getType(), SdlValue::BINARY_OPERATION);
	EXPECT_EQ(v1->getType(), SdlValue::BINARY_OPERATION);

	EXPECT_EQ(((SdlBinaryOperator *)v0)->getLeft()->getType(), SdlValue::NODE_REF);
	EXPECT_EQ(((SdlBinaryOperator *)v0)->getRight()->getType(), SdlValue::CONSTANT_LABEL);

	EXPECT_EQ(((SdlBinaryOperator *)v1)->getLeft()->getType(), SdlValue::BINARY_OPERATION);
	EXPECT_EQ(((SdlBinaryOperator *)v1)->getRight()->getType(), SdlValue::CONSTANT_LABEL);
}

TEST(SdlTests, SdlSingleNodeSimpleEq) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_simple_eq.mr");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
}

TEST(SdlTests, SdlImportTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_import_statement.mr");

	int importCount = parser.getImportStatementCount();

	EXPECT_EQ(importCount, 2);

	SdlImportStatement *a = parser.getImportStatement(0);
	SdlImportStatement *b = parser.getImportStatement(1);

	EXPECT_EQ(a->getLibName(), "test.sdl");
	EXPECT_EQ(b->getLibName(), "test");
}

TEST(SdlTests, SdlNodeDefinitionTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_definition.mr");

	EXPECT_EQ(parser.getNodeDefinitionCount(), 1);

	SdlNodeDefinition *nodeDef = parser.getNodeDefinition(0);
	EXPECT_EQ(nodeDef->getName(), "NewNode");
	EXPECT_EQ(nodeDef->isBuiltin(), false);
	EXPECT_EQ(nodeDef->getScope(), SdlNodeDefinition::EXPORT);

	const SdlAttributeDefinitionList *definitions =
		nodeDef->getAttributionDefinitionList();

	EXPECT_EQ(definitions->getDefinitionCount(), 4);
}

TEST(SdlTests, SdlNodeBuiltinTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_builtin.mr");

	EXPECT_EQ(parser.getNodeDefinitionCount(), 1);

	SdlNodeDefinition *nodeDef = parser.getNodeDefinition(0);
	EXPECT_EQ(nodeDef->getBuiltinName(), "BuiltinNode");
	EXPECT_EQ(nodeDef->getName(), "NewNode");
	EXPECT_EQ(nodeDef->isBuiltin(), true);
	EXPECT_EQ(nodeDef->getScope(), SdlNodeDefinition::EXPORT);

	const SdlAttributeDefinitionList *definitions = 
		nodeDef->getAttributionDefinitionList();

	EXPECT_EQ(definitions->getDefinitionCount(), 4);

	EXPECT_EQ(definitions->getDefinition(0)->isDefault(), true);
	EXPECT_EQ(definitions->getDefinition(0)->getDirection(),
		SdlAttributeDefinition::INPUT);
	EXPECT_EQ(definitions->getDefinition(0)->getName(), "A");
	CHECK_SDL_POS(definitions->getDefinition(0), 5, 20, 18, 18);

	EXPECT_EQ(definitions->getDefinition(1)->isDefault(), false);
	EXPECT_EQ(definitions->getDefinition(1)->getDirection(),
		SdlAttributeDefinition::INPUT);
	EXPECT_EQ(definitions->getDefinition(1)->getName(), "B");

	EXPECT_EQ(definitions->getDefinition(2)->isDefault(), true);
	EXPECT_EQ(definitions->getDefinition(2)->getDirection(),
		SdlAttributeDefinition::OUTPUT);
	EXPECT_EQ(definitions->getDefinition(2)->getName(), "A_out");

	EXPECT_EQ(definitions->getDefinition(3)->isDefault(), false);
	EXPECT_EQ(definitions->getDefinition(3)->getDirection(),
		SdlAttributeDefinition::OUTPUT);
	EXPECT_EQ(definitions->getDefinition(3)->getName(), "B_out");
}

TEST(SdlTests, SdlFloatTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_vector_float.mr");

	SdlNode *node = parser.getNode(0);
	SdlAttribute *testAttrib = node->getAttributes()->getAttribute(0);

	EXPECT_EQ(testAttrib->getName(), "test_float");

	SdlValue *value = testAttrib->getValue();
	EXPECT_EQ(value->getType(), SdlValue::NODE_REF);

	SdlValueNodeRef *nodeRef = static_cast<SdlValueNodeRef *>(value);
	node = nodeRef->getValue();
	SdlAttribute *attribute1 = node->getAttributes()->getAttribute(0);
	SdlAttribute *attribute2 = node->getAttributes()->getAttribute(1);
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::CONSTANT_FLOAT);
	EXPECT_EQ(attribute2->getValue()->getType(), SdlValue::CONSTANT_FLOAT);

	SdlValueFloat *v1 = (SdlValueFloat *)attribute1->getValue();
	SdlValueFloat *v2 = (SdlValueFloat *)attribute2->getValue();

	EXPECT_DOUBLE_EQ(v1->getValue(), 10.123);
	EXPECT_DOUBLE_EQ(v2->getValue(), 15.456);

	CHECK_SDL_POS(v1, 26, 32, 2, 2);
	CHECK_SDL_POS(v2, 34, 40, 2, 2);
}

TEST(SdlTests, SdlBoolTest) {
	SdlCompilationUnit parser;
	parser.parseFile(SDL_TEST_FILES "single_node_bool.mr");

	SdlNode *node = parser.getNode(0);
	SdlAttribute *falseAttrib = node->getAttributes()->getAttribute(0);
	SdlAttribute *trueAttrib = node->getAttributes()->getAttribute(1);

	EXPECT_EQ(falseAttrib->getName(), "test_false");
	EXPECT_EQ(trueAttrib->getName(), "test_true");

	SdlValue *falseValue = falseAttrib->getValue();
	SdlValue *trueValue = trueAttrib->getValue();

	EXPECT_EQ(falseValue->getType(), SdlValue::CONSTANT_BOOL);
	EXPECT_EQ(trueValue->getType(), SdlValue::CONSTANT_BOOL);

	SdlValueBool *castFalse = (SdlValueBool *)falseValue;
	SdlValueBool *castTrue = (SdlValueBool *)trueValue;

	EXPECT_EQ(castFalse->getValue(), false);
	EXPECT_EQ(castTrue->getValue(), true);

	CHECK_SDL_POS(castFalse, 17, 22, 2, 2);
	CHECK_SDL_POS(castTrue, 16, 20, 3, 3);
}

TEST(SdlTests, SdlSyntaxErrorTest) {
	SdlErrorList errorList;
	SdlCompilationUnit parser;
	parser.setErrorList(&errorList);
	parser.parseFile(SDL_TEST_FILES "syntax_error.mr");

	int errorCount = errorList.getErrorCount();
	EXPECT_EQ(errorCount, 3);

	SdlCompilationError *err1 = errorList.getCompilationError(0);
	SdlCompilationError *err2 = errorList.getCompilationError(1);
	SdlCompilationError *err3 = errorList.getCompilationError(2);

	CHECK_ERROR_CODE(err1, "S", "0001");
	CHECK_ERROR_CODE(err2, "S", "0001");
	CHECK_ERROR_CODE(err3, "P", "0001");
}

TEST(SdlTests, SdlCompilerTest) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.build(SDL_TEST_FILES "dependency_test.mr");

	int dependencyCount = unit->getDependencyCount();
	EXPECT_EQ(dependencyCount, 1);

	// Simple sanity check to make sure it's the right file
	SdlCompilationUnit *dep = unit->getDependency(0);
	EXPECT_EQ(dep->getNodeDefinitionCount(), 1);
}

TEST(SdlTests, SdlDependencyTreeTest) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.build(SDL_TEST_FILES "dependency_tree.mr");

	int dependencyCount = unit->getDependencyCount();
	EXPECT_EQ(dependencyCount, 2);

	// Simple sanity check to make sure it's the right file
	SdlCompilationUnit *dep = unit->getDependency(0);
	EXPECT_EQ(dep->getNodeDefinitionCount(), 1);

	SdlCompilationUnit *dep2 = unit->getDependency(1);
	EXPECT_EQ(dep2->getNodeDefinitionCount(), 2);

	int secondaryDependencyCount = dep2->getDependencyCount();
	EXPECT_EQ(secondaryDependencyCount, 1);

	SdlCompilationUnit *secondaryDep = dep2->getDependency(0);

	// Make sure that the compiler doesn't build a file twice
	EXPECT_EQ(dep, secondaryDep);
	EXPECT_EQ(compiler.getUnitCount(), 3);
}
