#include <pch.h>

#include <sdl_driver.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_value_label.h>
#include <sdl_value_node_ref.h>

#include "utilities.h"

using namespace manta;

TEST(SdlTests, SdlSanityCheck) {
	SdlDriver parser;
	parser.parseFile(SDL_TEST_FILES "single_empty_node.txt");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");
	EXPECT_EQ(attributes->getAttributeCount(), 0);
	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlSingleAttribute) {
	SdlDriver parser;
	parser.parseFile(SDL_TEST_FILES "single_node_single_attrib.txt");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute = attributes->getAttribute(0);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute->getName(), "test");
	EXPECT_EQ(attribute->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute->getValue())->getLabel(), "test");
	EXPECT_EQ(attributes->getAttributeCount(), 1);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlTwoAttributes) {
	SdlDriver parser;
	parser.parseFile(SDL_TEST_FILES "single_node_two_attribs.txt");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute0->getName(), "test1");
	EXPECT_EQ(attribute0->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute0->getValue())->getLabel(), "test");

	EXPECT_EQ(attribute1->getName(), "test2");
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute1->getValue())->getLabel(), "test");

	EXPECT_EQ(attributes->getAttributeCount(), 2);

	EXPECT_EQ(parser.getNodeCount(), 1);
}

TEST(SdlTests, SdlTwoNodes) {
	SdlDriver parser;
	parser.parseFile(SDL_TEST_FILES "two_nodes.txt");

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
	SdlDriver parser;
	parser.parseFile(SDL_TEST_FILES "single_node_inline_node.txt");

	const SdlNode *node = parser.getNode(0);
	const SdlAttributeList *attributes = node->getAttributes();
	const SdlAttribute *attribute0 = attributes->getAttribute(0);
	const SdlAttribute *attribute1 = attributes->getAttribute(1);

	EXPECT_EQ(node->getName(), "testImage");
	EXPECT_EQ(node->getType(), "FileImage");

	EXPECT_EQ(attribute0->getName(), "test1");
	EXPECT_EQ(attribute0->getValue()->getType(), SdlValue::CONSTANT_LABEL);
	EXPECT_EQ(((SdlValueLabel *)attribute0->getValue())->getLabel(), "test");

	EXPECT_EQ(attribute1->getName(), "test2");
	EXPECT_EQ(attribute1->getValue()->getType(), SdlValue::NODE_REF);

	SdlNode *inlineNode = ((SdlValueNodeRef *)attribute1->getValue())->getNodeRef();
	EXPECT_EQ(inlineNode->getType(), "InlineNode");
	EXPECT_EQ(inlineNode->getName(), "");
	EXPECT_EQ(inlineNode->getAttributes()->getAttributeCount(), 1);

	SdlNode *inlineNode2 = ((SdlValueNodeRef *)attributes->getAttribute(2)->getValue())->getNodeRef();
	EXPECT_EQ(inlineNode2->getType(), "InlineNode2");
	EXPECT_EQ(inlineNode2->getName(), "");
	EXPECT_EQ(inlineNode2->getAttributes()->getAttributeCount(), 1);

	EXPECT_EQ(attributes->getAttributeCount(), 3);

	EXPECT_EQ(parser.getNodeCount(), 1);
}
