#include <sdl_node.h>

#include <sdl_attribute.h>
#include <sdl_attribute_list.h>
#include <sdl_compilation_unit.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_definition.h>
#include <sdl_compilation_error.h>
#include <sdl_attribute_definition_list.h>

manta::SdlNode::SdlNode() {
	/* void */
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, SdlAttributeList *attributes) {
	m_type = type;
	m_name = name;
	m_attributes = attributes;

	registerToken(&type);
	registerToken(&name);
	registerComponent(attributes);

	m_definition = nullptr;
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes) {
	m_type = type;
	m_name = SdlTokenInfo_string("");
	m_attributes = attributes;

	registerToken(&type);
	registerComponent(attributes);
}

manta::SdlNode::~SdlNode() {
	/* void */
}

void manta::SdlNode::resolveNodeDefinition(SdlCompilationUnit *unit) {
	int definitionCount = 0;
	SdlNodeDefinition *definition = unit->resolveNodeDefinition(this, &definitionCount);

	if (definitionCount > 0) {
		// TODO: log a warning when a node type is ambiguous
	}

	if (definition == nullptr) {
		unit->addCompilationError(new SdlCompilationError(getNameToken(), 
			{ "R", "0001", "Undefined node type" }));
	}

	else {
		setDefinition(definition);
	}
}

void manta::SdlNode::resolveAttributeDefinitions(SdlCompilationUnit *unit) {
	if (m_definition == nullptr) {
		// The definition wasn't found so resolving any attributes doesn't make sense
		return;
	}

	if (m_attributes == nullptr) {
		// There was a compilation error in the attributes section, so this step can be skipped
		return;
	}

	int attributeCount = m_attributes->getAttributeCount();
	for (int i = 0; i < attributeCount; i++) {
		SdlAttribute *attribute = m_attributes->getAttribute(i);

		std::string name = attribute->getName();
		SdlAttributeDefinition *definition;
		
		if (name == "") {
			// Name not specified, positional notation is assumed
			const SdlAttributeDefinitionList *list = m_definition->getAttributeDefinitionList();
			int position = attribute->getPosition();

			// Check position is not out of bounds
			if (position >= list->getInputCount()) {
				unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(), 
					{ "R", "0004", "Position argument out of bounds" }));
				attribute->setAttributeDefinition(nullptr);
				return;
			}
			else {
				definition = m_definition->getAttributeDefinitionList()->getInputDefinition(
					attribute->getPosition()
				);
			}
		}
		else {
			definition = m_definition->getAttributeDefinition(attribute->getName());
		}

		if (definition == nullptr) {
			// Port not found
			unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(), 
				{ "R", "0003", "Port not found" }));
			attribute->setAttributeDefinition(nullptr);
		}
		else if (definition->getDirection() == SdlAttributeDefinition::OUTPUT) {
			// Can't assign an output port
			unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(), 
				{ "R", "0002", "Using an output port as in input" }));
			attribute->setAttributeDefinition(nullptr);
		}
		else {
			// Port is good
			attribute->setAttributeDefinition(definition);
		}
	}
}
