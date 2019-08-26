#include <pch.h>

#include "utilities.h"

#include "../include/binary_node.h"
#include "../include/vector_map_wrapper_node.h"
#include "../include/step_node.h"

#include "../include/manta_math.h"

using namespace manta;

TEST(NodeTests, StepNode) {
    VectorMap2D map;
    map.initialize(16, 16, math::constants::One);
    VectorMapWrapperNode wrapper(&map);
    wrapper.initialize();

    map.set(math::constants::Zero, 0, 0);
    map.set(math::constants::Double, 1, 0);

    StepNode node;
    node.initialize();

    node.getMainOutput()->setDefaultDc(math::constants::One);
    node.getMainOutput()->setDefaultFoot(math::constants::One);
    node.getMainOutput()->setDefaultStep(math::constants::Double);
    node.connectInput(wrapper.getMainOutput(), "Input", nullptr);

    node.evaluate();

    // Check state
    const VectorNodeOutput *o = static_cast<VectorNodeOutput *>(node.getOutput("Output"));
    EXPECT_EQ(o->getDimensions(), 1);
    EXPECT_EQ(o->getSize(0), 1);

    math::Vector result;
    o->discreteSample2d(0, 0, (void *)&result);
    CHECK_VEC_EQ(result, math::constants::One, 1E-4);

    o->discreteSample2d(1, 0, (void *)&result);
    CHECK_VEC_EQ(result, math::constants::Double, 1E-4);

    node.destroy();
    wrapper.destroy();
    map.destroy();
}
