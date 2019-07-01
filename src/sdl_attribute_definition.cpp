#include <sdl_attribute_definition.h>

#include <sdl_value.h>
#include <sdl_input_connection.h>
#include <sdl_compilation_unit.h>
#include <sdl_compilation_error.h>

manta::SdlAttributeDefinition::SdlAttributeDefinition(const SdlTokenInfo_string &directionToken,
								const SdlTokenInfo_string &name, DIRECTION dir) {
	m_name = name;
	registerToken(&m_name);

	m_directionToken = directionToken;
	registerToken(&m_directionToken);

	m_direction = dir;

	if (m_direction == OUTPUT) {
		setVisibility(SdlVisibility::PUBLIC);
	}
}

manta::SdlAttributeDefinition::SdlAttributeDefinition(const SdlTokenInfo_string &name) {
	m_name = name;
	registerToken(&m_name);

	m_direction = OUTPUT;
	setVisibility(SdlVisibility::PUBLIC);
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

manta::SdlParserStructure *manta::SdlAttributeDefinition::getImmediateReference(SdlParserStructure *inputContext, SdlCompilationError **err) {
	(void)inputContext;

	// An attribute definition will by default point to its definition (ie default value)
	return m_defaultValue;
}
