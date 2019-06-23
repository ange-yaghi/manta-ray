#include <sdl_node_definition.h>

#include <sdl_attribute_definition_list.h>
#include <sdl_attribute_definition.h>
#include <sdl_compilation_unit.h>
#include <sdl_node.h>
#include <sdl_compilation_error.h>

manta::SdlNodeDefinition::SdlNodeDefinition(const SdlTokenInfo_string &name) {
	m_name = name;
	registerToken(&name);

	// Node definitions are public by default
	setDefaultVisibility(SdlVisibility::PUBLIC);
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
	if (m_attributes == nullptr) return nullptr;

	int attributeCount = m_attributes->getDefinitionCount();

	for (int i = 0; i < attributeCount; i++) {
		SdlAttributeDefinition *definition = m_attributes->getDefinition(i);

		if (definition->getName() == attributeName) {
			return definition;
		}
	}

	return nullptr;
}

manta::SdlParserStructure *manta::SdlNodeDefinition::resolveName(const std::string &name) const {
	// Node definitions are not able to see variables outside of themselves for now
	return resolveLocalName(name);
}

int manta::SdlNodeDefinition::countSymbolIncidence(const std::string &name) const {
	int count = 0;
	if (m_attributes != nullptr) {
		int attributeCount = m_attributes->getDefinitionCount();
		for (int i = 0; i < attributeCount; i++) {
			SdlAttributeDefinition *definition = m_attributes->getDefinition(i);

			if (definition->getName() == name) {
				count++;
			}
		}
	}

	if (m_body != nullptr) {
		int nodeCount = m_body->getItemCount();
		for (int i = 0; i < nodeCount; i++) {
			SdlNode *node = m_body->getItem(i);
			if (node->getName() == name) {
				count++;
			}
		}
	}

	return count;
}

manta::SdlParserStructure *manta::SdlNodeDefinition::resolveLocalName(const std::string &name) const {
	if (m_attributes != nullptr) {
		int attributeCount = m_attributes->getDefinitionCount();
		for (int i = 0; i < attributeCount; i++) {
			SdlAttributeDefinition *definition = m_attributes->getDefinition(i);

			if (definition->getName() == name) {
				return definition;
			}
		}
	}

	if (m_body != nullptr) {
		int nodeCount = m_body->getItemCount();
		for (int i = 0; i < nodeCount; i++) {
			SdlNode *node = m_body->getItem(i);
			if (node->getName() == name) {
				return node;
			}
		}
	}

	return nullptr;
}

void manta::SdlNodeDefinition::_validate() {
	SdlCompilationUnit *unit = getParentUnit();

	// Check that no symbol is used more than once
	if (m_attributes != nullptr) {
		int attributeCount = m_attributes->getDefinitionCount();
		for (int i = 0; i < attributeCount; i++) {
			SdlAttributeDefinition *definition = m_attributes->getDefinition(i);
			int incidence = countSymbolIncidence(definition->getName());
			if (incidence > 1) {
				unit->addCompilationError(new SdlCompilationError(*definition->getNameToken(),
					SdlErrorCode::SymbolUsedMultipleTimes));
			}
		}
	}

	if (m_body != nullptr) {
		int nodeCount = m_body->getItemCount();
		for (int i = 0; i < nodeCount; i++) {
			SdlNode *node = m_body->getItem(i);
			int incidence = countSymbolIncidence(node->getName());
			if (incidence > 1) {
				unit->addCompilationError(new SdlCompilationError(node->getNameToken(),
					SdlErrorCode::SymbolUsedMultipleTimes));
			}
		}
	}

	// Check that every output has a definition unless this is a built-in type
	if (m_attributes != nullptr) {
		int attributeCount = m_attributes->getDefinitionCount();
		for (int i = 0; i < attributeCount; i++) {
			SdlAttributeDefinition *definition = m_attributes->getDefinition(i);
			if (definition->getDirection() == SdlAttributeDefinition::OUTPUT) {
				if (definition->getDefaultValue() == nullptr && !isBuiltin()) {
					unit->addCompilationError(new SdlCompilationError(*definition->getNameToken(),
						SdlErrorCode::OutputWithNoDefinition));
				}
				else if (definition->getDefaultValue() != nullptr && isBuiltin()) {
					unit->addCompilationError(new SdlCompilationError(*definition->getNameToken(),
						SdlErrorCode::BuiltinOutputWithDefinition));
				}
			}
		}
	}
}
