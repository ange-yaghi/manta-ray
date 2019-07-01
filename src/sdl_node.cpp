#include <sdl_node.h>

#include <sdl_attribute.h>
#include <sdl_attribute_list.h>
#include <sdl_compilation_unit.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_definition.h>
#include <sdl_compilation_error.h>
#include <sdl_attribute_definition_list.h>
#include <sdl_value.h>
#include <node.h>
#include <custom_node.h>

#include <standard_allocator.h>
#include <srgb_node.h>
#include <constructed_vector_node.h>

manta::SdlNode::SdlNode() {
	/* void */
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, 
			SdlAttributeList *attributes, const SdlTokenInfo_string &library) {
	m_type = type;
	m_name = name;
	m_attributes = attributes;
	m_library = library;

	registerToken(&type);
	registerToken(&name);
	registerToken(&library);
	registerComponent(attributes);

	m_definition = nullptr;
	m_generatedNode = nullptr;
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes,
			const SdlTokenInfo_string &library) {
	m_type = type;
	m_name = SdlTokenInfo_string("");
	m_attributes = attributes;
	m_library = library;

	registerToken(&type);
	registerToken(&library);
	registerComponent(attributes);

	m_generatedNode = nullptr;
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

manta::SdlParserStructure *manta::SdlNode::resolveLocalName(const std::string &name) const {
	SdlParserStructure *attribute = getAttribute(name);
	if (attribute != nullptr) return attribute;
	
	if (m_definition != nullptr) return m_definition->resolveLocalName(name);
	else return nullptr;
}

void manta::SdlNode::_resolveDefinitions() {
	resolveNodeDefinition();
	resolveAttributeDefinitions();
}

void manta::SdlNode::_validate() {
	SdlAttributeList *attributes = m_attributes;
	SdlCompilationUnit *unit = getParentUnit();

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

void manta::SdlNode::_checkInstantiation() {
	// Check all references relating to the connection of inputs from this
	// node to the actual definition.
	if (m_definition != nullptr) {
		m_definition->checkReferences(this);
	}
}

void manta::SdlNode::resolveNodeDefinition() {
	int definitionCount = 0;
	SdlNodeDefinition *definition = nullptr;
	SdlCompilationUnit *unit = getParentUnit();

	if (m_library.specified) {
		if (!m_library.data.empty()) {
			definition = unit->resolveNodeDefinition(getType(), &definitionCount, m_library.data);
		}
		else {
			// Adding an empty library name means that the local scope must strictly be used
			definition = unit->resolveLocalNodeDefinition(getType(), &definitionCount);
		}
	}
	else {
		definition = unit->resolveNodeDefinition(getType(), &definitionCount, "");
	}

	if (definitionCount > 0) {
		// TODO: log a warning when a node type is ambiguous
	}

	if (definition == nullptr) {
		unit->addCompilationError(new SdlCompilationError(getTypeToken(), 
			SdlErrorCode::UndefinedNodeType));
	}

	else {
		setDefinition(definition);
	}
}

void manta::SdlNode::resolveAttributeDefinitions() {
	if (m_definition == nullptr) {
		// The definition wasn't found so resolving any attributes doesn't make sense
		return;
	}

	if (m_attributes == nullptr) {
		// There was a compilation error in the attributes section, so this step can be skipped
		return;
	}

	SdlCompilationUnit *unit = getParentUnit();

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

manta::Node *manta::SdlNode::generateNode() {
	if (m_generatedNode != nullptr) return m_generatedNode;

	SdlNodeDefinition *definition = getDefinition();
	const SdlAttributeDefinitionList *allAttributes = definition->getAttributeDefinitionList();
	const SdlAttributeList *specifiedAttributes = getAttributes();
	int attributeCount = allAttributes->getDefinitionCount();

	struct Mapping {
		std::string name;
		NodeOutput *output;
		bool primary;
	};

	std::vector<Mapping> inputs;
	std::vector<Mapping> outputs;

	for (int i = 0; i < attributeCount; i++) {
		SdlAttributeDefinition *attributeDefinition = allAttributes->getDefinition(i);

		if (attributeDefinition->getDirection() == SdlAttributeDefinition::INPUT) {
			SdlAttribute *attribute = specifiedAttributes->getAttribute(attributeDefinition);

			if (attribute != nullptr) {
				// Input was specified
				SdlValue *asValue = attribute->getImmediateReference(this, nullptr)->getAsValue();

				Mapping inputPort;
				inputPort.output = asValue->generateNodeOutput(this);
				inputPort.name = attributeDefinition->getName();
				inputs.push_back(inputPort);
			}
			else {
				// Use the default value in the definition
				SdlValue *asValue = attributeDefinition->getImmediateReference(this, nullptr)->getAsValue();

				Mapping inputPort;
				inputPort.output = asValue->generateNodeOutput(this);
				inputPort.name = attributeDefinition->getName();
				inputs.push_back(inputPort);
			}
		}
		else if (attributeDefinition->getDirection() == SdlAttributeDefinition::OUTPUT &&
																	!definition->isBuiltin()) {
			SdlValue *asValue = attributeDefinition->getImmediateReference(this, nullptr)->getAsValue();

			Mapping outputPort;
			outputPort.output = asValue->generateNodeOutput(this);
			outputPort.name = attributeDefinition->getName();
			outputPort.primary = asValue == getDefaultOutputValue();

			outputs.push_back(outputPort);
		}
	}

	int inputCount = (int)inputs.size();
	int outputCount = (int)outputs.size();

	Node *newNode = nullptr;
	if (definition->isBuiltin()) {
		if (definition->getBuiltinName() == "__builtin__vector") {
			newNode = StandardAllocator::Global()->allocate<ConstructedVectorNode>();
			newNode->initialize();
		}

		if (definition->getBuiltinName() == "__builtin__srgb") {
			newNode = StandardAllocator::Global()->allocate<SrgbNode>();
			newNode->initialize();
		}
	}
	else {
		CustomNode *newCustomNode = StandardAllocator::Global()->allocate<CustomNode>();

		for (int i = 0; i < inputCount; i++) {
			newCustomNode->addCustomInput(inputs[i].name);
		}

		for (int i = 0; i < outputCount; i++) {
			newCustomNode->addCustomOutput(outputs[i].output, outputs[i].name, outputs[i].primary);
		}

		newCustomNode->initialize();
		newNode = newCustomNode;
	}

	if (newNode != nullptr) {
		for (int i = 0; i < inputCount; i++) {
			newNode->connectInput(inputs[i].output, inputs[i].name.c_str());
		}
	}

	m_generatedNode = newNode;
	return newNode;
}

manta::SdlValue *manta::SdlNode::getDefaultOutputValue() {
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
