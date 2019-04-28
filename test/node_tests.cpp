#include <pch.h>

#include "utilities.h"

#include <multiply_node.h>
#include <vector_map_wrapper_node.h>

#include <manta_math.h>

using namespace manta;

TEST(NodeTests, MultiplyNodeTest) {
	MultiplyNode node;
	node.initialize();

	node.getMainOutput()->setDefaultA(math::constants::One);
	node.getMainOutput()->setDefaultB(math::constants::Double);

	node.evaluate();

	// Check state
	NodeOutput *o = node.getOutput("Output");
	EXPECT_EQ(o->getDimensions(), 1);
	EXPECT_EQ(o->getSize(0), 1);

	math::Vector result;
	o->discreteSample2D(0, 0, (void *)&result);
	CHECK_VEC_EQ(result, math::constants::Double, 1E-4);

	node.destroy();
}

TEST(NodeTests, MultiplyMapNodeTest) {
	VectorMap2D map;
	map.initialize(16, 16, math::constants::One);
	VectorMapWrapperNode wrapper(&map);
	wrapper.initialize();

	MultiplyNode node;
	node.initialize();

	node.connectInput(wrapper.getOutput("Output"), "A");
	node.getMainOutput()->setDefaultB(math::constants::Double);

	node.evaluate();

	// Check state
	NodeOutput *o = node.getOutput("Output");
	EXPECT_EQ(o->getDimensions(), 2);
	EXPECT_EQ(o->getSize(0), 16);
	EXPECT_EQ(o->getSize(1), 16);

	math::Vector result;
	o->discreteSample2D(0, 0, (void *)&result);
	CHECK_VEC_EQ(result, math::constants::Double, 1E-4);

	node.destroy();
	wrapper.destroy();

	map.destroy();
}
