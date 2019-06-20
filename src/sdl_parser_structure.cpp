#include <sdl_parser_structure.h>

#include <sdl_compilation_unit.h>

manta::SdlParserStructure::SdlParserStructure() {
	m_parentScope = nullptr;
	m_checkReferences = true;

	m_definitionsResolved = false;
	m_validated = false;

	m_externalAccess = false;
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

		m_components.push_back(child);
	}
}

manta::SdlParserStructure *manta::SdlParserStructure::resolveName(const std::string &name) const {
	SdlParserStructure *local = resolveLocalName(name);
	if (local != nullptr) return local;
	
	if (m_parentScope != nullptr) {
		return m_parentScope->resolveName(name);
	}

	return nullptr;
}

manta::SdlParserStructure *manta::SdlParserStructure::getReference(SdlCompilationError **err) {
	if (err != nullptr) *err = nullptr;
	SdlParserStructure *immediateRefernece = getImmediateReference(err);

	// Error checking is not done on any parent nodes because it's assumed that errors have
	// already been checked/reported
	if (immediateRefernece != nullptr) return immediateRefernece->getReference(nullptr);
	else return this;
}

void manta::SdlParserStructure::resolveDefinitions(SdlCompilationUnit *unit) {
	if (m_definitionsResolved) return;

	// Resolve components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->resolveDefinitions(unit);
	}

	_resolveDefinitions(unit);

	m_definitionsResolved = true;
}

void manta::SdlParserStructure::checkReferences(SdlCompilationUnit *unit) {
	// Check components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->checkReferences(unit);
	}

	if (m_checkReferences) {
		SdlCompilationError *err = nullptr;
		SdlParserStructure *reference = getReference(&err);

		if (err != nullptr) {
			unit->addCompilationError(err);
		}
	}
}

void manta::SdlParserStructure::validate(SdlCompilationUnit *unit) {
	if (m_validated) return;

	// Validate components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->validate(unit);
	}

	_validate(unit);

	m_validated = true;
}

void manta::SdlParserStructure::expand(SdlCompilationUnit *unit) {
	if (m_isExpanded) return;

	// Expand components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->expand(unit);
	}

	_expand(unit);

	if (m_expansion != nullptr) {
		// If this structure is found to result in an expanded form,
		// then it will also be assumed that it references that expansion
		//m_reference = m_expansion;
		//m_referencesResolved = true;

		registerComponent(m_expansion);
	}

	m_isExpanded = true;
}

void manta::SdlParserStructure::_validate(SdlCompilationUnit *unit) {
	/* void */
}

void manta::SdlParserStructure::_resolveDefinitions(SdlCompilationUnit *unit) {
	/* void */
}

void manta::SdlParserStructure::_expand(SdlCompilationUnit *unit) {
	m_expansion = nullptr;
}

manta::SdlParserStructure *manta::SdlParserStructure::resolveLocalName(const std::string &name) const {
	return nullptr;
}
