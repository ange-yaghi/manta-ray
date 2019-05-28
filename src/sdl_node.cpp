#include <sdl_node.h>

manta::SdlNode::SdlNode() {
	m_type = "";
	m_name = "";
}

manta::SdlNode::SdlNode(const std::string &type, const std::string &name, SdlAttributeList *attributes) {
	m_type = type;
	m_name = name;
	m_attributes = attributes;
}

manta::SdlNode::~SdlNode() {
	/* void */
}
