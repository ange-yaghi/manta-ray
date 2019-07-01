#include <sdl_generator.h>

#include <sdl_node.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_value.h>
#include <node.h>
#include <constructed_vector_node.h>
#include <srgb_node.h>
#include <sdl_attribute_definition_list.h>
#include <standard_allocator.h>

manta::SdlGenerator::SdlGenerator() {
	/* void */
}

manta::SdlGenerator::~SdlGenerator() {
	/* void */
}

manta::Node *manta::SdlGenerator::generateNode(SdlNode *reference) const {
	return nullptr;
}
