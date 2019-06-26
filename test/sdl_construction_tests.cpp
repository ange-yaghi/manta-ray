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

#include "utilities.h"

using namespace manta;


TEST(SdlConstructionTests, SdlVectorTest) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.compile(SDL_TEST_FILES "manta_lib/tests/vector_test.mr");
	EXPECT_NE(unit, nullptr);

	const SdlErrorList *errors = compiler.getErrorList();

	SdlNode *node = unit->getNode(0);
	
	EXPECT_EQ(node->getType(), "vector");
	EXPECT_EQ(node->getName(), "test_vector");

	SdlGenerator generator;
	Node *generatedNode = generator.generateNode(node);

	math::Vector test;
	generatedNode->getOutput("vector")->sample(nullptr, (void *)&test);

	CHECK_VEC_EQ(test, math::loadVector(1.0f, 2.0f, 3.0f, 4.0f), 1E-5);

	math::Vector x, y, z, w;
	generatedNode->getOutput("x")->sample(nullptr, (void *)&x);
	generatedNode->getOutput("y")->sample(nullptr, (void *)&y);
	generatedNode->getOutput("z")->sample(nullptr, (void *)&z);
	generatedNode->getOutput("w")->sample(nullptr, (void *)&w);

	CHECK_VEC_EQ(x, math::loadScalar(1.0), 1E-5);
	CHECK_VEC_EQ(y, math::loadScalar(2.0), 1E-5);
	CHECK_VEC_EQ(z, math::loadScalar(3.0), 1E-5);
	CHECK_VEC_EQ(w, math::loadScalar(4.0), 1E-5);
}
