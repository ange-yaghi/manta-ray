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

	if (count != nullptr) {
		*count = 0;
	}

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

/*
manta::SdlParserStructure *manta::SdlNode::getPublicAttribute(const std::string &name, bool *failed) const {
	if (failed != nullptr) *failed = false;

	auto definition = m_definition;
	if (definition == nullptr) return nullptr;

	auto definitionList = definition->getAttributeDefinitionList();
	if (definitionList == nullptr) return nullptr;

	auto outputDefinition = definitionList->getOutputDefinition(name);
	if (outputDefinition == nullptr) {
		// In this case it is a true failure because rather than there being a syntax error,
		// the output name itself was not found
		if (failed != nullptr) *failed = true;
		return nullptr;
	}

	auto defaultValue = outputDefinition->getDefaultValue();
	if (defaultValue == nullptr) return nullptr;

	return defaultValue;
}*/

manta::SdlParserStructure *manta::SdlNode::resolveLocalName(const std::string &name) const {
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

	int attributeCount = attributes->getAttributeCount();
	for (int i = 0; i < attributeCount; i++) {
		SdlAttribute *attribute = attributes->getAttribute(i);

		// Only check if this is not a positional attribute
		if (attribute->isPositional()) continue;

		int count;
		SdlAttribute *other = getAttribute(attribute->getName(), &count);
		if (count > 1) {
			// Attribute defined multiple times
			unit->addCompilationError(new SdlCompilationError(*attribute->getSummaryToken(),
				SdlErrorCode::InputSpecifiedMultipleTimes));
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
