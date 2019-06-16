#include <sdl_node.h>

#include <sdl_attribute.h>
#include <sdl_attribute_list.h>
#include <sdl_compilation_unit.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_definition.h>
#include <sdl_compilation_error.h>
#include <sdl_attribute_definition_list.h>
#include <sdl_value.h>

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

void manta::SdlNode::setAttributes(SdlAttributeList *list) {
	m_attributes = list;
	registerComponent(list);
}

manta::SdlAttribute *manta::SdlNode::getAttribute(const std::string &name, int *count) const {
	if (m_attributes == nullptr) return nullptr;

	if (count != nullptr) *count = 0;

	SdlAttribute *result = nullptr;
	int attributeCount = m_attributes->getAttributeCount();
	for (int i = 0; i < attributeCount; i++) {
		SdlAttribute *attribute = m_attributes->getAttribute(i);
		if (attribute->getName() == name) {
			if (result == nullptr) result = attribute;
			if (count != nullptr) (*count)++;
		}
	}

	return result;
}

manta::SdlAttribute *manta::SdlNode::getAttribute(SdlAttributeDefinition *definition, int *count) const {
	if (m_attributes == nullptr) return nullptr;

	if (count != nullptr) *count = 0;

	SdlAttribute *result = nullptr;
	int attributeCount = m_attributes->getAttributeCount();
	for (int i = 0; i < attributeCount; i++) {
		SdlAttribute *attribute = m_attributes->getAttribute(i);
		if (attribute->getAttributeDefinition() == definition) {
			if (result == nullptr) result = attribute;
			if (count != nullptr) (*count)++;
		}
	}

	return result;
}

manta::SdlParserStructure *manta::SdlNode::resolveLocalName(const std::string &name) {
	SdlParserStructure *attribute = getAttribute(name);
	if (attribute != nullptr) return attribute;
	
	return m_definition->resolveLocalName(name);
}

void manta::SdlNode::_resolveDefinitions(SdlCompilationUnit *unit) {
	resolveNodeDefinition(unit);
	resolveAttributeDefinitions(unit);
}

void manta::SdlNode::_validate(SdlCompilationUnit *unit) {
	SdlAttributeList *attributes = m_attributes;

	if (attributes == nullptr) {
		// There was a syntax error before this step
		return;
	}

	// Check for symbols used more than once
	int attributeCount = attributes->getAttributeCount();
	for (int i = 0; i < attributeCount; i++) {
		SdlAttribute *attribute = attributes->getAttribute(i);

		int count;
		bool positional = attribute->isPositional();

		SdlAttributeDefinition *definition = attribute->getAttributeDefinition();

		// If there was an error resolving the definition, skip this validation step
		if (definition != nullptr) getAttribute(definition, &count);
		else continue;

		if (count > 1) {
			// Attribute defined multiple times

			if (positional) {
				// Log a more specific message for clarify if the attribute is positional
				unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(),
					SdlErrorCode::InputSpecifiedMultipleTimesPositional));
			}
			else {
				unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(),
					SdlErrorCode::InputSpecifiedMultipleTimes));
			}
		}
	}

	// Check for missing inputs
	if (m_definition != nullptr) {
		auto attributeList = m_definition->getAttributeDefinitionList();

		// Check that there wasn't an error in the definition
		if (attributeList != nullptr) {
			int inputCount = attributeList->getInputCount();

			for (int i = 0; i < inputCount; i++) {
				SdlAttributeDefinition *input = attributeList->getInputDefinition(i);
				SdlParserStructure *attribute = getAttribute(input);

				if (attribute == nullptr && input->getDefaultValue() == nullptr) {
					// This input port is not conencted and has no default value
					unit->addCompilationError(new SdlCompilationError(*getSummaryToken(),
						SdlErrorCode::InputNotConnected));
				}
			}
		}
	}
}

void manta::SdlNode::resolveNodeDefinition(SdlCompilationUnit *unit) {
	int definitionCount = 0;
	SdlNodeDefinition *definition = unit->resolveNodeDefinition(this, &definitionCount);

	if (definitionCount > 0) {
		// TODO: log a warning when a node type is ambiguous
	}

	if (definition == nullptr) {
		unit->addCompilationError(new SdlCompilationError(getNameToken(), 
			SdlErrorCode::UndefinedNodeType));
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

		SdlAttributeDefinition *definition;
		
		if (attribute->isPositional()) {
			const SdlAttributeDefinitionList *list = m_definition->getAttributeDefinitionList();
			int position = attribute->getPosition();

			// Check position is not out of bounds
			if (position >= list->getInputCount()) {
				unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(), 
					SdlErrorCode::ArgumentPositionOutOfBounds));
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
				SdlErrorCode::PortNotFound));
			attribute->setAttributeDefinition(nullptr);
		}
		else if (definition->getDirection() == SdlAttributeDefinition::OUTPUT) {
			// Can't assign an output port
			unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(), 
				SdlErrorCode::UsingOutputPortAsInput));
			attribute->setAttributeDefinition(nullptr);
		}
		else {
			// Port is good
			attribute->setAttributeDefinition(definition);
		}
	}
}

manta::SdlValue *manta::SdlNode::getAsValue() {
	auto definition = m_definition;
	if (definition == nullptr) return nullptr;

	auto definitionList = definition->getAttributeDefinitionList();
	if (definitionList == nullptr) return nullptr;

	auto outputDefinition = definitionList->getDefaultOutput();
	if (outputDefinition == nullptr) return nullptr;

	auto defaultValue = outputDefinition->getDefaultValue();
	if (defaultValue == nullptr) return nullptr;

	return defaultValue->getAsValue();
}
