#include <sdl_node_definition.h>

#include <sdl_attribute_definition_list.h>
#include <sdl_attribute_definition.h>

manta::SdlNodeDefinition::SdlNodeDefinition(const SdlTokenInfo_string &name) {
	m_name = name;
	registerToken(&name);
}

manta::SdlNodeDefinition::~SdlNodeDefinition() {
	/* void */
}

void manta::SdlNodeDefinition::setBuiltinName(const SdlTokenInfo_string &builtinName) {
	m_builtinName = builtinName;
	registerToken(&m_builtinName);
}

void manta::SdlNodeDefinition::setAttributeDefinitionList(SdlAttributeDefinitionList *definitions) {
	m_attributes = definitions;
	registerComponent(definitions);
}

void manta::SdlNodeDefinition::setScopeToken(const SdlTokenInfo_string &scopeToken) {
	m_scopeToken = scopeToken;
	registerToken(&scopeToken);
}

manta::SdlAttributeDefinition *manta::SdlNodeDefinition::getAttributeDefinition(const std::string &attributeName) const {
	int attributeCount = m_attributes->getDefinitionCount();

	for (int i = 0; i < attributeCount; i++) {
		SdlAttributeDefinition *definition = m_attributes->getDefinition(i);

		if (definition->getName() == attributeName) {
			return definition;
		}
	}

	return nullptr;
}
