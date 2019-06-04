#include <sdl_attribute_definition_list.h>

#include <sdl_attribute_definition.h>

manta::SdlAttributeDefinitionList::SdlAttributeDefinitionList() {
	/* void */
}

manta::SdlAttributeDefinitionList::~SdlAttributeDefinitionList() {
	/* void */
}

void manta::SdlAttributeDefinitionList::addDefinition(SdlAttributeDefinition *definition) {
	m_definitions.push_back(definition);
	registerComponent(definition);
}
