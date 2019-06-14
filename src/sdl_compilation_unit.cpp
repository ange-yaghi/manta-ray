#include <sdl_compilation_unit.h>

#include <sdl_scanner.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_error_list.h>

#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream>

manta::SdlCompilationUnit::~SdlCompilationUnit() {
	delete m_scanner;
	m_scanner = nullptr;

	delete m_parser;
	m_parser = nullptr;
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parseFile(const Path &filename, SdlCompilationUnit *topLevel) {
	m_path = filename;
	
	std::ifstream inFile(filename.toString());
	if (!inFile.good()) {
		return IO_ERROR;
	}

	return parseHelper(inFile);
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parse(const char *sdl, SdlCompilationUnit *topLevel) {
	std::istringstream sdlStream(sdl);
	return parse(sdlStream);
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parse(std::istream &stream, SdlCompilationUnit *topLevel) {
	if (!stream.good() && stream.eof()) {
		return IO_ERROR;
	}

	return parseHelper(stream);
}

void manta::SdlCompilationUnit::resolve() {
	if (isResolved()) return;
	else m_resolved = true;

	// Resolve dependencies first
	int dependencyCount = getDependencyCount();
	for (int i = 0; i < dependencyCount; i++) {
		m_dependencies[i]->resolve();
	}

	// Resolve all references to node definitions
	resolveNodeDefinitions();
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parseHelper(
									std::istream &stream, SdlCompilationUnit *topLevel) {
	delete m_scanner;
	try {
		m_scanner = new manta::SdlScanner(&stream);
	}
	catch (std::bad_alloc) {
		return FAIL;
	}

	delete m_parser;
	try {
		m_parser = new manta::SdlParser(*m_scanner, *this);
	}
	catch (std::bad_alloc) {
		return FAIL;
	}

	const int success = 0;
	if (m_parser->parse() != success) {
		std::cerr << "Parse failed!!" << std::endl;
		return FAIL;
	}
	else {
		return SUCCESS;
	}
}

void manta::SdlCompilationUnit::resolveNodeDefinitions() {
	int nodeCount = getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		SdlNode *node = m_nodes[i];
		node->resolveNodeDefinition(this);
		node->resolveAttributeDefinitions(this);
	}

	int nodeDefinitionCount = getNodeDefinitionCount();
	for (int i = 0; i < nodeDefinitionCount; i++) {
		SdlNodeDefinition *definition = getNodeDefinition(i);
		definition->resolveNodeDefinitions(this);
	}
}

void manta::SdlCompilationUnit::resolveReferences() {
	int nodeCount = getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		SdlNode *node = m_nodes[i];
		node->resolveReferences();
	}

	int nodeDefinitionCount = getNodeDefinitionCount();
	for (int i = 0; i < nodeDefinitionCount; i++) {
		SdlNodeDefinition *definition = getNodeDefinition(i);
		definition->resolveReferences();
	}
}

manta::SdlNodeDefinition *manta::SdlCompilationUnit::resolveNodeDefinition(SdlNode *node, int *count, bool searchDependencies) {
	*count = 0;
	manta::SdlNodeDefinition *definition = nullptr;
	std::string typeName = node->getType();

	// First search local node definitions
	int localNodeDefinitions = (int)m_nodeDefinitions.size();
	for (int i = 0; i < localNodeDefinitions; i++) {
		std::string defName = m_nodeDefinitions[i]->getName();

		if (defName == typeName) {
			(*count)++;
			if (definition == nullptr) definition = m_nodeDefinitions[i];
		}
	}

	// Search dependencies
	if (searchDependencies) {
		int dependencyCount = getDependencyCount();
		for (int i = 0; i < dependencyCount; i++) {
			int secondaryCount = 0;
			SdlNodeDefinition *def = m_dependencies[i]->resolveNodeDefinition(node, &secondaryCount, false);
			if (def != nullptr) {
				(*count) += secondaryCount;
				if (definition == nullptr) definition = def;
			}
		}
	}

	return definition;
}

void manta::SdlCompilationUnit::addNode(SdlNode *node) {
	m_nodes.push_back(node);
	registerComponent(node);
}

manta::SdlNode *manta::SdlCompilationUnit::getNode(int index) const {
	return m_nodes[index];
}

int manta::SdlCompilationUnit::getNodeCount() const {
	return (int)m_nodes.size();
}

void manta::SdlCompilationUnit::addImportStatement(SdlImportStatement *statement) {
	m_importStatements.push_back(statement);
	registerComponent(statement);
}

int manta::SdlCompilationUnit::getImportStatementCount() const {
	return (int)m_importStatements.size();
}

void manta::SdlCompilationUnit::addNodeDefinition(SdlNodeDefinition *nodeDefinition) {
	m_nodeDefinitions.push_back(nodeDefinition);
	registerComponent(nodeDefinition);
}

int manta::SdlCompilationUnit::getNodeDefinitionCount() const {
	return (int)m_nodeDefinitions.size();
}

void manta::SdlCompilationUnit::addCompilationError(SdlCompilationError *err) {
	if (m_errorList != nullptr) {
		err->setCompilationUnit(this);
		m_errorList->addCompilationError(err);
	}
	else {
		delete err;
	}
}

std::ostream& manta::SdlCompilationUnit::print(std::ostream &stream) {
	return stream;
}
