#include <sdl_attribute.h>

#include <sdl_value.h>

manta::SdlAttribute::SdlAttribute() {
	/* void */
}

manta::SdlAttribute::SdlAttribute(const SdlTokenInfo_string &name, SdlValue *value) {
	m_name = name;
	m_value = value;

	registerToken(&name);
	registerComponent(value);
}

manta::SdlAttribute::SdlAttribute(SdlValue *value) {
	m_value = value;

	registerComponent(value);
}

manta::SdlAttribute::~SdlAttribute() {
	/* void */
}
