#include <sdl_value_label.h>

manta::SdlValueLabel::SdlValueLabel(const std::string &label) : SdlValue(CONSTANT_LABEL) {
	m_label = label;
}

manta::SdlValueLabel::~SdlValueLabel() {
	/* void */
}
