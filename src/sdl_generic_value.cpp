#include <sdl_generic_value.h>

#include <sdl_compilation_unit.h>
#include <sdl_compilation_error.h>
#include <sdl_node_definition.h>

manta::SdlGenericValue::SdlGenericValue(const SdlTokenInfoSet<std::string, 2> &type) 
														: SdlValue(SdlValue::GENERIC) {
	m_typeInfo = type;
	m_typeDefinition = nullptr;
}

manta::SdlGenericValue::~SdlGenericValue() {
	/* void */
}

manta::SdlParserStructure *manta::SdlGenericValue::resolveLocalName(const std::string &name) const {
	if (m_typeDefinition != nullptr) {
		return m_typeDefinition->resolveLocalName(name);
	}
	else return nullptr;
}

void manta::SdlGenericValue::_resolveDefinitions() {
	int definitionCount = 0;
	SdlNodeDefinition *definition = nullptr;
	SdlCompilationUnit *unit = getParentUnit();

	const SdlTokenInfo_string &libraryToken = m_typeInfo.data[0];
	const SdlTokenInfo_string &typeToken = m_typeInfo.data[1];

	const std::string &library = libraryToken.data;
	const std::string &type = typeToken.data;

	if (libraryToken.specified) {
		if (!library.empty()) {
			definition = unit->resolveNodeDefinition(type, &definitionCount, library);
		}
		else {
			// Adding an empty library name means that the local scope must strictly be used
			definition = unit->resolveLocalNodeDefinition(type, &definitionCount);
		}
	}
	else {
		if (typeToken.specified) {
			definition = unit->resolveNodeDefinition(type, &definitionCount, "");
		}
		else {
			// Type information was not given so skip the rest of the function
			m_typeDefinition = nullptr;
			return;
		}
	}

	if (definitionCount > 0) {
		// TODO: log a warning when a node type is ambiguous
	}

	if (definition == nullptr) {
		unit->addCompilationError(new SdlCompilationError(typeToken,
			SdlErrorCode::UndefinedNodeType));
		m_typeDefinition = nullptr;
	}

	else {
		m_typeDefinition = definition;
	}
}
