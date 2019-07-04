#include <sdl_attribute.h>

#include <sdl_value.h>

manta::SdlAttribute::SdlAttribute() {
	/* void */
}

manta::SdlAttribute::SdlAttribute(const SdlTokenInfo_string &name, SdlValue *value) {
	m_name = name;
	m_value = value;
	m_position = -1;

	registerToken(&name);
	registerComponent(value);

	m_definition = nullptr;
}

manta::SdlAttribute::SdlAttribute(SdlValue *value) {
	m_value = value;
	m_position = -1;

	registerComponent(value);
}

manta::SdlAttribute::~SdlAttribute() {
	/* void */
}

void manta::SdlAttribute::setValue(SdlValue *value) {
	m_value = value;
	registerComponent(value);
}

manta::SdlParserStructure *manta::SdlAttribute::getImmediateReference(const SdlReferenceQuery &query, SdlReferenceInfo *output) {
	SDL_INFO_OUT(err, nullptr);
	SDL_INFO_OUT(failed, false);

	return m_value;
}
