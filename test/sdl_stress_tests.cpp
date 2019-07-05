#include <pch.h>

#include <node.h>
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
#include <sdl_generator.h>
#include <rgb_space.h>
#include <material_manager.h>

#include "utilities.h"

using namespace manta;

const SdlErrorList *compileFile(const std::string &filename) {
	SdlCompiler *compiler = new SdlCompiler();
	SdlCompilationUnit *unit = compiler->compile(SDL_TEST_FILES + filename);
	EXPECT_NE(unit, nullptr);

	const SdlErrorList *errors = compiler->getErrorList();

	SdlNode *node = unit->getNode(0);
	std::ofstream f(TMP_PATH + std::string("trace.txt"));
	node->writeTraceToFile(f);
	f.close();

	return errors;
}

TEST(SdlSyntaxStressTests, SdlSyntaxStressTest1) {
	const SdlErrorList *errors = compileFile("stress-testing/stress_test_1.mr");

	EXPECT_EQ(errors->getErrorCount(), 13);

	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 50));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 51));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 52));

	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 54));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 55));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UnresolvedReference, 56));

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 60));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 62));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 64));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 66));

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 68));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 69));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 74));
}

TEST(SdlSyntaxStressTests, SdlSyntaxNodeArgumentStressTest1) {
	const SdlErrorList *errors = compileFile("stress-testing/node_argument_stress_test_1.mr");

	EXPECT_EQ(errors->getErrorCount(), 5);

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 22, nullptr, true));
	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 23, nullptr));
}

TEST(SdlSyntaxStressTests, SdlSyntaxDeepErrorIsolated) {
	const SdlErrorList *errors = compileFile("stress-testing/deep_error_isolated.mr");

	EXPECT_EQ(errors->getErrorCount(), 1);

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 3, nullptr, true));
}

TEST(SdlSyntaxStressTests, SdlSyntaxDeepError) {
	const SdlErrorList *errors = compileFile("stress-testing/deep_error.mr");

	EXPECT_EQ(errors->getErrorCount(), 3);

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 27, nullptr, true));
}
