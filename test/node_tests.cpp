#include <pch.h>

#include "utilities.h"

#include <multiply_node.h>
#include <vector_map_wrapper_node.h>
#include <step_node.h>

#include <manta_math.h>

using namespace manta;

TEST(NodeTests, MultiplyNodeTest) {
	MultiplyNode node;
	node.initialize();

	node.getMainOutput()->setDefaultA(math::constants::One);
	node.getMainOutput()->setDefaultB(math::constants::Double);

	node.evaluate();

	// Check state
	const NodeOutput *o = node.getOutput("Output");
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
	const NodeOutput *o = node.getOutput("Output");
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
	node.connectInput(wrapper.getMainOutput(), "Input");

	node.evaluate();

	// Check state
	const NodeOutput *o = node.getOutput("Output");
	EXPECT_EQ(o->getDimensions(), 1);
	EXPECT_EQ(o->getSize(0), 1);

	math::Vector result;
	o->discreteSample2D(0, 0, (void *)&result);
	CHECK_VEC_EQ(result, math::constants::One, 1E-4);

	o->discreteSample2D(1, 0, (void *)&result);
	CHECK_VEC_EQ(result, math::constants::Double, 1E-4);

	node.destroy();
	wrapper.destroy();
	map.destroy();
}
