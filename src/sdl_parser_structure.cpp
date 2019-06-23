#include <sdl_parser_structure.h>

#include <sdl_compilation_unit.h>

manta::SdlParserStructure::SdlParserStructure() {
	m_parentScope = nullptr;
	m_checkReferences = true;

	m_definitionsResolved = false;
	m_validated = false;

	m_visibility = SdlVisibility::DEFAULT;
	m_defaultVisibility = SdlVisibility::PRIVATE;
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

manta::SdlParserStructure *manta::SdlParserStructure::getReference(SdlParserStructure *inputContext, SdlCompilationError **err) {
	if (err != nullptr) *err = nullptr;
	SdlParserStructure *immediateReference = getImmediateReference(inputContext, err);

	// Error checking is not done on any parent nodes because it's assumed that errors have
	// already been checked/reported
	if (immediateReference != nullptr) return immediateReference->getReference(inputContext, nullptr);
	else return this;
}

void manta::SdlParserStructure::resolveDefinitions() {
	if (m_definitionsResolved) return;

	// Resolve components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->resolveDefinitions();
	}

	_resolveDefinitions();

	m_definitionsResolved = true;
}

void manta::SdlParserStructure::checkReferences(SdlParserStructure *inputContext) {
	// Check components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->checkReferences(inputContext);
	}

	if (m_checkReferences) {
		SdlCompilationError *err = nullptr;
		SdlParserStructure *reference = getReference(inputContext, &err);

		if (err != nullptr) {
			getParentUnit()->addCompilationError(err);
		}
	}

	_checkInstantiation();
}

void manta::SdlParserStructure::validate() {
	if (m_validated) return;

	// Validate components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->validate();
	}

	_validate();

	m_validated = true;
}

void manta::SdlParserStructure::expand() {
	if (m_isExpanded) return;

	// Expand components
	int componentCount = getComponentCount();
	for (int i = 0; i < componentCount; i++) {
		m_components[i]->expand();
	}

	_expand();

	if (m_expansion != nullptr) {
		// If this structure is found to result in an expanded form,
		// then it will also be assumed that it references that expansion
		//m_reference = m_expansion;
		//m_referencesResolved = true;

		registerComponent(m_expansion);
	}

	m_isExpanded = true;
}

void manta::SdlParserStructure::_validate() {
	/* void */
}

void manta::SdlParserStructure::_checkInstantiation() {
	/* void */
}

void manta::SdlParserStructure::_resolveDefinitions() {
	/* void */
}

void manta::SdlParserStructure::_expand() {
	m_expansion = nullptr;
}

manta::SdlParserStructure *manta::SdlParserStructure::resolveLocalName(const std::string &name) const {
	return nullptr;
}

bool manta::SdlParserStructure::allowsExternalAccess() const {
	SdlVisibility visibility = (m_visibility == SdlVisibility::DEFAULT) ? m_defaultVisibility : m_visibility;
	return visibility == SdlVisibility::PUBLIC;
}

manta::SdlCompilationUnit *manta::SdlParserStructure::getParentUnit() const {
	if (m_parentUnit == nullptr) return m_parentScope->getParentUnit();
	else return m_parentUnit;
}
