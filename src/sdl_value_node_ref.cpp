#include <sdl_value_node_ref.h>

#include <sdl_value.h>

manta::SdlValueNodeRef::SdlValueNodeRef(SdlNode *nodeRef) : SdlValue(NODE_REF) {
	m_nodeRef = nodeRef;
}

manta::SdlValueNodeRef::~SdlValueNodeRef() {
	/* void */
}
