#include <sdl_attribute_definition.h>

#include <sdl_value.h>
#include <sdl_input_connection.h>

manta::SdlAttributeDefinition::SdlAttributeDefinition(const SdlTokenInfo_string &directionToken,
								const SdlTokenInfo_string &name, DIRECTION dir) {
	m_name = name;
	registerToken(&m_name);

	m_directionToken = directionToken;
	registerToken(&m_directionToken);

	m_direction = dir;

	if (m_direction == OUTPUT) {
		setExternalAccess(true);
	}
}

manta::SdlAttributeDefinition::~SdlAttributeDefinition() {
	/* void */
}

void manta::SdlAttributeDefinition::setDefaultValue(SdlValue *value) {
	m_defaultValue = value;
	registerComponent(m_defaultValue);
}

void manta::SdlAttributeDefinition::setDefaultToken(const SdlTokenInfo_string &defaultToken) {
	m_defaultToken = defaultToken;
	registerToken(&m_defaultToken);
}

manta::SdlInputConnection *manta::SdlAttributeDefinition::getImpliedMember(const std::string &name) const {
	int count = getImpliedMemberCount();

	for (int i = 0; i < count; i++) {
		if (m_impliedMembers[i]->getMember() == name) {
			return m_impliedMembers[i];
		}
	}

	return nullptr;
}

manta::SdlParserStructure *manta::SdlAttributeDefinition::resolveLocalName(const std::string &name) {
	// If this function is called it means that a member of this attribute
	// is being accessed. Ordinarily this would be a member of an input. Since
	// we can't figure out right away what this actually references (since a node
	// definition can be instantiated multiple times) we return a placeholder with
	// the member in question.
	SdlInputConnection *inputConnection = getImpliedMember(name);
	if (inputConnection != nullptr) return inputConnection;
	else {
		inputConnection = new SdlInputConnection();
		inputConnection->setInputAttribute(this);
		inputConnection->setMember(name);
		inputConnection->setParentScope(this);

		registerComponent(inputConnection);

		m_impliedMembers.push_back(inputConnection);

		return inputConnection;
	}
}

manta::SdlParserStructure *manta::SdlAttributeDefinition::resolveName(const std::string &name) {
	if (m_parentScope != nullptr) {
		return m_parentScope->resolveName(name);
	}

	return nullptr;
}

void manta::SdlAttributeDefinition::_resolveReferences(SdlCompilationUnit *unit) {
	// An attribute definition will by default point to its definition (ie default value)
	m_reference = m_defaultValue;
}
