#include <sdl_parser_structure.h>

manta::SdlParserStructure::SdlParserStructure() {
	m_parentScope = nullptr;
}

manta::SdlParserStructure::~SdlParserStructure() {
	/* void */
}

void manta::SdlParserStructure::registerToken(const SdlTokenInfo *tokenInfo) {
	if (tokenInfo != nullptr) m_summaryToken.combine(tokenInfo);
}

void manta::SdlParserStructure::registerComponent(SdlParserStructure *child) {
	if (child != nullptr) {
		m_summaryToken.combine(child->getSummaryToken());
		child->setParentScope(this);
	}
}

manta::SdlParserStructure *manta::SdlParserStructure::resolveName(const std::string &name) const {
	SdlParserStructure *local = resolveLocalName();
	if (local != nullptr) return local;
	
	if (m_parentScope != nullptr) {
		return m_parentScope->resolveName(name);
	}

	return nullptr;
}

void manta::SdlParserStructure::resolve() {
	if (m_isResolved) return;

	m_isResolved = true;
	_resolve();
}

void manta::SdlParserStructure::expand() {
	if (m_isExpanded) return;

	m_isExpanded = true;
	_expand();
}

void manta::SdlParserStructure::_resolve() {
	m_reference = this;
}

void manta::SdlParserStructure::_expand() {
	m_expansion = nullptr;
}

manta::SdlParserStructure *manta::SdlParserStructure::resolveLocalName() const {
	return nullptr;
}
