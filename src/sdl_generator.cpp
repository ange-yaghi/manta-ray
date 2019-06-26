#include <sdl_generator.h>

#include <sdl_node.h>
#include <sdl_node_definition.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_value.h>
#include <node.h>
#include <vector_node.h>
#include <sdl_attribute_definition_list.h>
#include <standard_allocator.h>

manta::SdlGenerator::SdlGenerator() {
	/* void */
}

manta::SdlGenerator::~SdlGenerator() {
	/* void */
}

manta::Node *manta::SdlGenerator::generateNode(SdlNode *reference) const {
	SdlNodeDefinition *definition = reference->getDefinition();
	const SdlAttributeDefinitionList *allAttributes = definition->getAttributeDefinitionList();
	const SdlAttributeList *specifiedAttributes = reference->getAttributes();
	int attributeCount = allAttributes->getInputCount();

	struct Mapping {
		std::string name;
		NodeOutput *output;
	};

	std::vector<Mapping> inputs;
	std::vector<Mapping> outputs;

	for (int i = 0; i < attributeCount; i++) {
		SdlAttributeDefinition *attributeDefinition = allAttributes->getDefinition(i);
		
		if (attributeDefinition->getDirection() == SdlAttributeDefinition::INPUT) {
			SdlAttribute *attribute = specifiedAttributes->getAttribute(attributeDefinition);

			if (attribute != nullptr) {
				// Input was specified
				SdlValue *asValue = attribute->getReference()->getAsValue();

				Mapping inputPort;
				inputPort.output = asValue->generateNodeOutput();
				inputPort.name = attributeDefinition->getName();
				inputs.push_back(inputPort);
			}
			else {
				// Use the default value in the definition
				SdlValue *asValue = definition->getReference()->getAsValue();

				Mapping inputPort;
				inputPort.output = asValue->generateNodeOutput();
				inputPort.name = attributeDefinition->getName();
				inputs.push_back(inputPort);
			}
		}
		else if (attributeDefinition->getDirection() == SdlAttributeDefinition::OUTPUT &&
																!definition->isBuiltin()) {
			SdlValue *asValue = definition->getReference()->getAsValue();

			Mapping inputPort;
			inputPort.output = asValue->generateNodeOutput();
			inputPort.name = attributeDefinition->getName();
			outputs.push_back(inputPort);
		}
	}

	Node *newNode = nullptr;
	if (definition->isBuiltin()) {
		if (definition->getBuiltinName() == "__builtin__vector") {
			newNode = StandardAllocator::Global()->allocate<VectorNode>();
			newNode->initialize();
		}
	}

	if (newNode != nullptr) {
		int inputCount = (int)inputs.size();
		int outputCount = (int)outputs.size();

		for (int i = 0; i < inputCount; i++) {
			newNode->connectInput(inputs[i].output, inputs[i].name.c_str());
		}

		for (int i = 0; i < outputCount; i++) {

		}
	}

	return newNode;
}
