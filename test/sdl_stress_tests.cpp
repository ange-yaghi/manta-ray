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

	return errors;
}

TEST(SdlSyntaxStressTests, SdlSyntaxStressTest1) {
	const SdlErrorList *errors = compileFile("stress-testing/stress_test_1.mr");

	EXPECT_EQ(errors->getErrorCount(), 1);

	EXPECT_TRUE(findError(errors, SdlErrorCode::UndefinedMember, 40));
}