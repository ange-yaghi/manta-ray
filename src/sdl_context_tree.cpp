#include <sdl_context_tree.h>

#include <sdl_node.h>
#include <sdl_attribute_definition.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>

manta::SdlContextTree::SdlContextTree() {
	m_context = nullptr;
	m_parent = nullptr;
}

manta::SdlContextTree::SdlContextTree(SdlNode *context, bool mainContext) {
	m_context = context;
	m_mainContext = mainContext;
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

manta::SdlContextTree *manta::SdlContextTree::newChild(SdlNode *context, bool mainContext) {
	SdlContextTree *newChild = new SdlContextTree(context, mainContext);
	newChild->setParent(this);

	m_children.push_back(newChild);

	return newChild;
}

manta::SdlContextTree *manta::SdlContextTree::findContext(SdlParserStructure *context) {
	if (m_context == context) return this;
	else if (m_parent != nullptr) return m_parent->findContext(context);
	else return nullptr;
}

manta::SdlParserStructure *manta::SdlContextTree::resolveDefinition(SdlAttributeDefinition *definition) {
	if (m_context != nullptr) {
		return m_context->getAttributes()->getAttribute(definition);
	}

	return nullptr;
}
