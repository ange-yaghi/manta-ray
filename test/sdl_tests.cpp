#include <pch.h>

#include "utilities.h"
#include "../include/vector_node_output.h"

TEST(SdlTests, SdlSanityCheck) {
    const piranha::ErrorList *errList;
    piranha::IrCompilationUnit *unit = compileFile("basic_float_test.mr", &errList);

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();

    piranha::Node *node = program.getNode(0);
    piranha::native_float data;
    node->getPrimaryOutput()->fullCompute((void *)&data);

    EXPECT_EQ(data, 5.0);
}

TEST(SdlTests, SdlVectorTest) {
    const piranha::ErrorList *errList;
    piranha::IrCompilationUnit *unit = compileFile("basic_vector_test.mr", &errList);

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();

    piranha::Node *node = program.getNode(12);
    math::Vector data;
    static_cast<VectorNodeOutput *>(node->getPrimaryOutput())->sample(nullptr, (void *)&data);

    CHECK_VEC(data, 1.0, 2.0, 3.0, 4.0);

    piranha::Node *scalar = program.getNode(15);
    static_cast<VectorNodeOutput *>(scalar->getPrimaryOutput())->sample(nullptr, (void *)&data);

    CHECK_VEC(data, 666.0, 666.0, 666.0, 666.0);
}

TEST(SdlTests, SdlStringTest) {
    const piranha::ErrorList *errList;
    piranha::IrCompilationUnit *unit = compileFile("basic_string_test.mr", &errList);

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();

    piranha::Node *node = program.getNode(0);
    std::string data;
    node->getPrimaryOutput()->fullCompute((void *)&data);

    EXPECT_EQ(data, "Hello world");
}

TEST(SdlTests, SdlPhongTest) {
    const piranha::ErrorList *errList;
    piranha::IrCompilationUnit *unit = compileFile("basic_phong_test.mr", &errList);

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();
}

TEST(SdlTests, SdlSimpleMaterialTest) {
	const piranha::ErrorList *errList;
	piranha::IrCompilationUnit *unit = compileFile("basic_bsdf_test.mr", &errList);

	piranha::NodeProgram program;
	unit->build(&program);

	program.execute();
}

TEST(SdlTests, SdlObjectNodeTest) {
    const piranha::ErrorList *errList;
    piranha::IrCompilationUnit *unit = compileFile("basic_object_file_test.mr", &errList);

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();

    int a = 0;
}
