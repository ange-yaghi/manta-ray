#include <sdl_attribute.h>

manta::SdlAttribute::SdlAttribute() {
	m_name = "";
}

manta::SdlAttribute::SdlAttribute(const std::string &name, SdlValue *value) {
	m_name = name;
	m_value = value;
}

manta::SdlAttribute::~SdlAttribute() {
	/* void */
}
