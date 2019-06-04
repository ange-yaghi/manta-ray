#include <sdl_node.h>

#include <sdl_attribute_list.h>

manta::SdlNode::SdlNode() {
	/* void */
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, const SdlTokenInfo_string &name, SdlAttributeList *attributes) {
	m_type = type;
	m_name = name;
	m_attributes = attributes;

	registerToken(&type);
	registerToken(&name);
	registerComponent(attributes);
}

manta::SdlNode::SdlNode(const SdlTokenInfo_string &type, SdlAttributeList *attributes) {
	m_type = type;
	m_name = SdlTokenInfo_string("");
	m_attributes = attributes;

	registerToken(&type);
	registerComponent(attributes);
}

manta::SdlNode::~SdlNode() {
	/* void */
}
