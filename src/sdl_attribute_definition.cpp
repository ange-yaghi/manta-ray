#include <sdl_attribute_definition.h>

#include <sdl_value.h>
#include <sdl_input_connection.h>
#include <sdl_compilation_unit.h>
#include <sdl_compilation_error.h>
#include <sdl_context_tree.h>

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

manta::SdlParserStructure *manta::SdlAttributeDefinition::getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output) {
	SDL_RESET(query);

	// First check the input context for the reference
	if (query.inputContext != nullptr) {
		SdlParserStructure *reference = query.inputContext->resolveDefinition(this);
		if (reference != nullptr) {
			SDL_INFO_OUT(newContext, query.inputContext->getParent());
			SDL_INFO_OUT(staticReference, false);

			return reference;
		}
	}

	// An attribute definition will by default point to its definition (ie default value)
	if (m_defaultValue == nullptr) {
		if (query.inputContext == nullptr) {
			SDL_DEAD_END();
			return nullptr;
		}
		else {
			// This is the result of an unconnected input (that has no input)
			SDL_FAIL();
			return nullptr;
		}
	}

	return m_defaultValue;
}
