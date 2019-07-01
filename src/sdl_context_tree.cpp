#include <sdl_context_tree.h>

manta::SdlContextTree::SdlContextTree() {
	m_context = nullptr;
	m_parent = nullptr;
}

manta::SdlContextTree::SdlContextTree(SdlParserStructure *context) {
	m_context = context;
}

manta::SdlContextTree::~SdlContextTree() {
	/* void */
}

manta::SdlContextTree *manta::SdlContextTree::getRoot() {
	if (m_parent != nullptr) return m_parent->getRoot();
	else return this;
}

manta::SdlContextTree *manta::SdlContextTree::search(const SdlContextTree *reference) const {
	return nullptr;
}

manta::SdlContextTree *manta::SdlContextTree::newChild(SdlParserStructure *context) {
	SdlContextTree *newChild = new SdlContextTree(context);
	newChild->setParent(this);

	m_children.push_back(newChild);

	return newChild;
}

manta::SdlContextTree *manta::SdlContextTree::findContext(SdlParserStructure *context) {
	if (m_context == context) return this;
	else if (m_parent != nullptr) return m_parent->findContext(context);
	else return nullptr;
}
