#include <sdl_attribute_definition_list.h>

#include <sdl_attribute_definition.h>

manta::SdlAttributeDefinitionList::SdlAttributeDefinitionList() {
	/* void */
}

manta::SdlAttributeDefinitionList::~SdlAttributeDefinitionList() {
	/* void */
}

void manta::SdlAttributeDefinitionList::addDefinition(SdlAttributeDefinition *definition) {
	if (definition != nullptr) {
		m_definitions.push_back(definition);
		registerComponent(definition);
	}
}

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getDefinition(int index, SdlAttributeDefinition::DIRECTION direction) const {
	int totalCount = getDefinitionCount();
	int inputIndex = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == direction) {
			if (inputIndex == index) {
				return definition;
			}

			inputIndex++;
		}
	}

	return nullptr;
}

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getInputDefinition(int index) const {
	return getDefinition(index, SdlAttributeDefinition::INPUT);
}

int manta::SdlAttributeDefinitionList::getCount(SdlAttributeDefinition::DIRECTION direction) const {
	int totalCount = getDefinitionCount();
	int inputs = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == direction) {
			inputs++;
		}
	}

	return inputs;
}

int manta::SdlAttributeDefinitionList::getInputCount() const {
	return getCount(SdlAttributeDefinition::INPUT);
}

int manta::SdlAttributeDefinitionList::getOutputCount() const {
	return getCount(SdlAttributeDefinition::OUTPUT);
}

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getOutputDefinition(int index) const {
	return getDefinition(index, SdlAttributeDefinition::OUTPUT);
}

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getOutputDefinition(const std::string &name) const {
	int totalCount = getDefinitionCount();
	int inputs = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == SdlAttributeDefinition::OUTPUT && name == definition->getName()) {
			return definition;
		}
	}

	return nullptr;
}

manta::SdlAttributeDefinition *manta::SdlAttributeDefinitionList::getDefaultOutput() const {
	SdlAttributeDefinition *firstOutput = nullptr;

	int totalCount = getDefinitionCount();
	int inputs = 0;
	for (int i = 0; i < totalCount; i++) {
		SdlAttributeDefinition *definition = m_definitions[i];
		if (definition->getDirection() == SdlAttributeDefinition::OUTPUT) {
			firstOutput = definition;

			if (definition->isDefault()) {
				return definition;
			}
		}
	}

	// If not explicit default is found, return the first output
	return firstOutput;
}
