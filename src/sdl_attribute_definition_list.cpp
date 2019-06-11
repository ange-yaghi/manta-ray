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

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getInputDefinition(int index) const {
	int totalCount = getDefinitionCount();
	int inputIndex = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == SdlAttributeDefinition::INPUT) {
			if (inputIndex == index) {
				return definition;
			}

			inputIndex++;
		}
	}

	return nullptr;
}

int manta::SdlAttributeDefinitionList::getInputCount() const {
	int totalCount = getDefinitionCount();
	int inputs = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == SdlAttributeDefinition::INPUT) {
			inputs++;
		}
	}

	return inputs;
}
