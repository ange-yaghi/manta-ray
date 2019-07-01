#include <sdl_value.h>

#include <node.h>

manta::SdlValue::SdlValue(manta::SdlValue::VALUE_TYPE type) {
	m_type = type;
}

manta::SdlValue::~SdlValue() {
	/* void */
}

manta::NodeOutput *manta::SdlValue::generateNodeOutput(SdlParserStructure *context) {
	GenerationTableEntry *entry = getEntry(context);
	if (entry == nullptr) entry = newEntry(context);

	if (entry->nodeReference == nullptr) {
		entry->nodeReference = generateNode(context);
	}

	if (entry->nodeReference != nullptr) {
		return entry->nodeReference->getPrimaryOutput();
	}

	if (entry->nodeGeneratedOutput == nullptr) {
		entry->nodeGeneratedOutput = _generateNodeOutput(context);
	}

	return entry->nodeGeneratedOutput;
}

manta::Node *manta::SdlValue::generateNode(SdlParserStructure *context) {
	GenerationTableEntry *entry = getEntry(context);
	if (entry == nullptr) entry = newEntry(context);

	if (entry->nodeReference == nullptr) {
		entry->nodeReference = _generateNode(context);
		return entry->nodeReference;
	}
	else return entry->nodeReference;
}

manta::NodeOutput *manta::SdlValue::_generateNodeOutput(SdlParserStructure *context) {
	return nullptr;
}

manta::Node *manta::SdlValue::_generateNode(SdlParserStructure *context) {
	return nullptr;
}

manta::SdlValue::GenerationTableEntry *manta::SdlValue::getEntry(SdlParserStructure *context) {
	int entryCount = (int)m_generationTable.size();
	for (int i = 0; i < entryCount; i++) {
		if (m_generationTable[i].context == context) {
			return &m_generationTable[i];
		}
	}

	return nullptr;
}

manta::SdlValue::GenerationTableEntry *manta::SdlValue::newEntry(SdlParserStructure *context) {
	GenerationTableEntry *newEntry = new GenerationTableEntry();
	newEntry->context = context;
	newEntry->nodeGeneratedOutput = nullptr;
	newEntry->nodeReference = nullptr;

	return newEntry;
}
