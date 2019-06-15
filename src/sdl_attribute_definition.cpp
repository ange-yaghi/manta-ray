#include <sdl_attribute_definition.h>

#include <sdl_value.h>

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

void manta::SdlAttributeDefinition::_resolveReferences(SdlCompilationUnit *unit) {
	// An attribute definition will by default point to its definition (ie default value)
	m_reference = m_defaultValue;
}
