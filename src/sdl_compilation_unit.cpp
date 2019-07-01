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

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parseFile(const Path &filename, 
																		SdlCompilationUnit *topLevel) {
	m_path = filename;
	
	std::ifstream inFile(filename.toString());
	if (!inFile.good()) {
		return IO_ERROR;
	}

	return parseHelper(inFile);
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parse(const char *sdl, 
															SdlCompilationUnit *topLevel) {
	std::istringstream sdlStream(sdl);
	return parse(sdlStream);
}

manta::SdlCompilationUnit::ParseResult manta::SdlCompilationUnit::parse(std::istream &stream, 
																SdlCompilationUnit *topLevel) {
	if (!stream.good() && stream.eof()) {
		return IO_ERROR;
	}

	return parseHelper(stream);
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

manta::SdlNodeDefinition *manta::SdlCompilationUnit::resolveNodeDefinition(const std::string &name, int *count,
														const std::string &libraryName, bool external) {
	*count = 0;
	manta::SdlNodeDefinition *definition = nullptr;
	std::string typeName = name;

	// First search local node definitions if a library is not specified
	if (libraryName.empty()) {
		int localCount = 0;
		SdlNodeDefinition *localDefinition = resolveLocalNodeDefinition(typeName, &localCount, external);
		(*count) += localCount;

		if (localDefinition != nullptr) return localDefinition;
	}

	// Search dependencies
	int dependencyCount = getImportStatementCount();
	for (int i = 0; i < dependencyCount; i++) {
		int secondaryCount = 0;
		SdlImportStatement *importStatement = getImportStatement(i);

		if (importStatement->getUnit() == nullptr) {
			// This import statement already failed, skip it
			continue;
		}

		if (	libraryName.empty() ||
				(importStatement->hasShortName() && libraryName == importStatement->getShortName()))
		{
			if (external) {
				// Check if the import statement is private or public
				if (!importStatement->allowsExternalAccess()) {
					continue;
				}
			}

			// The external access flag must be set to true since the libraries are being accessed
			// externally                                                                                         ----
			SdlNodeDefinition *def = importStatement->getUnit()->resolveNodeDefinition(name, &secondaryCount, "", true);
			if (def != nullptr) {
				(*count) += secondaryCount;
				if (definition == nullptr) definition = def;
			}
		}
	}

	return definition;
}

manta::SdlNodeDefinition *manta::SdlCompilationUnit::resolveLocalNodeDefinition(const std::string &name, int *count, bool external) {
	*count = 0;
	manta::SdlNodeDefinition *definition = nullptr;
	std::string typeName = name;

	int localNodeDefinitions = (int)m_nodeDefinitions.size();
	for (int i = 0; i < localNodeDefinitions; i++) {
		SdlNodeDefinition *def = m_nodeDefinitions[i];
		if (external && !def->allowsExternalAccess()) continue;

		std::string defName = m_nodeDefinitions[i]->getName();

		if (defName == typeName) {
			(*count)++;
			if (definition == nullptr) definition = m_nodeDefinitions[i];
		}
	}

	return definition;
}

void manta::SdlCompilationUnit::_validate() {
	int nodeCount = getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		SdlNode *node = m_nodes[i];
		int count = countSymbolIncidence(node->getName());

		if (count > 1) {
			this->addCompilationError(new SdlCompilationError(node->getNameToken(),
				SdlErrorCode::SymbolUsedMultipleTimes));
		}
	}

	int definitionCount = getNodeDefinitionCount();
	for (int i = 0; i < definitionCount; i++) {
		SdlNodeDefinition *def = m_nodeDefinitions[i];
		int count = 0;
		resolveLocalNodeDefinition(def->getName(), &count);

		if (count > 1) {
			this->addCompilationError(new SdlCompilationError(*def->getNameToken(),
				SdlErrorCode::DuplicateNodeDefinition));
		}
	}
}

void manta::SdlCompilationUnit::addNode(SdlNode *node) {
	if (node != nullptr) {
		m_nodes.push_back(node);
		registerComponent(node);

		node->setParentUnit(this);
	}
}

manta::SdlNode *manta::SdlCompilationUnit::getNode(int index) const {
	return m_nodes[index];
}

int manta::SdlCompilationUnit::getNodeCount() const {
	return (int)m_nodes.size();
}

void manta::SdlCompilationUnit::addImportStatement(SdlImportStatement *statement) {
	if (statement != nullptr) {
		m_importStatements.push_back(statement);
		registerComponent(statement);

		statement->setParentUnit(this);
	}
}

int manta::SdlCompilationUnit::getImportStatementCount() const {
	return (int)m_importStatements.size();
}

void manta::SdlCompilationUnit::addNodeDefinition(SdlNodeDefinition *nodeDefinition) {
	if (nodeDefinition != nullptr) {
		m_nodeDefinitions.push_back(nodeDefinition);
		registerComponent(nodeDefinition);

		nodeDefinition->setParentUnit(this);
	}
}

int manta::SdlCompilationUnit::getNodeDefinitionCount() const {
	return (int)m_nodeDefinitions.size();
}

manta::SdlParserStructure *manta::SdlCompilationUnit::resolveLocalName(const std::string &name) const {
	int nodeCount = getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		SdlNode *node = m_nodes[i];
		if (node->getName() == name) {
			return node;
		}
	}

	return nullptr;
}

int manta::SdlCompilationUnit::countSymbolIncidence(const std::string &name) const {
	int count = 0;
	int nodeCount = getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		SdlNode *node = m_nodes[i];
		if (node->getName() == name) {
			count++;
		}
	}

	return count;
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
