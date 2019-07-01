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

#include "utilities.h"

using namespace manta;


TEST(SdlConstructionTests, SdlVectorTest) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.compile(SDL_TEST_FILES "manta_lib/tests/vector_test.mr");
	EXPECT_NE(unit, nullptr);

	const SdlErrorList *errors = compiler.getErrorList();
	EXPECT_EQ(errors->getErrorCount(), 0);

	SdlNode *node = unit->getNode(0);
	
	EXPECT_EQ(node->getType(), "vector");
	EXPECT_EQ(node->getName(), "test_vector");

	Node *generatedNode = node->generateNode();

	math::Vector test;
	generatedNode->getPrimaryOutput()->sample(nullptr, (void *)&test);

	CHECK_VEC_EQ(test, math::loadVector(1.0f, 2.0f, 3.0f, 4.0f), 1E-5);

	math::Vector x, y, z, w;
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("x")->sample(nullptr, (void *)&x);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("y")->sample(nullptr, (void *)&y);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("z")->sample(nullptr, (void *)&z);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("w")->sample(nullptr, (void *)&w);

	CHECK_VEC_EQ(x, math::loadScalar(1.0), 1E-5);
	CHECK_VEC_EQ(y, math::loadScalar(2.0), 1E-5);
	CHECK_VEC_EQ(z, math::loadScalar(3.0), 1E-5);
	CHECK_VEC_EQ(w, math::loadScalar(4.0), 1E-5);
}

TEST(SdlConstructionTests, SdlVectorTest2) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.compile(SDL_TEST_FILES "manta_lib/tests/vector_test_2.mr");
	EXPECT_NE(unit, nullptr);

	const SdlErrorList *errors = compiler.getErrorList();
	EXPECT_EQ(errors->getErrorCount(), 0);

	SdlNode *node = unit->getNode(1);

	EXPECT_EQ(node->getType(), "vector");
	EXPECT_EQ(node->getName(), "test_vector");

	Node *generatedNode = node->generateNode();

	math::Vector test;
	generatedNode->getPrimaryOutput()->sample(nullptr, (void *)&test);

	CHECK_VEC_EQ(test, math::loadVector(1.0f, 2.0f, 3.0f, 4.0f), 1E-5);

	math::Vector x, y, z, w;
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("x")->sample(nullptr, (void *)&x);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("y")->sample(nullptr, (void *)&y);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("z")->sample(nullptr, (void *)&z);
	generatedNode->getPrimaryOutput()->getInterface()->getOutput("w")->sample(nullptr, (void *)&w);

	CHECK_VEC_EQ(x, math::loadScalar(1.0), 1E-5);
	CHECK_VEC_EQ(y, math::loadScalar(2.0), 1E-5);
	CHECK_VEC_EQ(z, math::loadScalar(3.0), 1E-5);
	CHECK_VEC_EQ(w, math::loadScalar(4.0), 1E-5);
}

TEST(SdlConstructionTests, SdlColorTest) {
	SdlCompiler compiler;
	SdlCompilationUnit *unit = compiler.compile(SDL_TEST_FILES "manta_lib/tests/color_test.mr");
	EXPECT_NE(unit, nullptr);

	const SdlErrorList *errors = compiler.getErrorList();
	EXPECT_EQ(errors->getErrorCount(), 0);

	SdlNode *node = unit->getNode(0);

	EXPECT_EQ(node->getType(), "sRGB");
	EXPECT_EQ(node->getName(), "c");

	SdlNode *vectorNode = unit->getNode(2);

	EXPECT_EQ(vectorNode->getType(), "vector");
	EXPECT_EQ(vectorNode->getName(), "v2");

	Node *vectorGeneratedNode = vectorNode->generateNode();

	math::Vector out;
	vectorGeneratedNode->getPrimaryOutput()->sample(nullptr, (void *)&out);

	math::Vector expected = math::loadVector(
		(math::real)RgbSpace::srgb.applyGammaSrgb(0.5),
		(math::real)RgbSpace::srgb.applyGammaSrgb(0.75),
		0.0f,
		0.0f
	);

	CHECK_VEC_EQ(out, expected, 1E-5);
}
