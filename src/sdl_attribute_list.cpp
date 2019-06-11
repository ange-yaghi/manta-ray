#include <sdl_attribute_list.h>

#include <sdl_attribute.h>

manta::SdlAttributeList::SdlAttributeList() {
	/* void */
}

manta::SdlAttributeList::~SdlAttributeList() {
	/* void */
}

void manta::SdlAttributeList::addAttribute(SdlAttribute *attribute) {
	int index = getAttributeCount();

	m_attributes.push_back(attribute);
	attribute->setPosition(index);

	registerComponent(attribute);
}
