#include <sdl_attribute_list.h>

#include <sdl_attribute.h>

manta::SdlAttributeList::SdlAttributeList() {
	/* void */
}

manta::SdlAttributeList::~SdlAttributeList() {
	/* void */
}

void manta::SdlAttributeList::addAttribute(SdlAttribute *attribute) {
	m_attributes.push_back(attribute);

	registerComponent(attribute);
}
