#include <sdl_compilation_unit.h>

#include <sdl_scanner.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>

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

void manta::SdlCompilationUnit::parseFile(const Path &rootDir, const Path &filename, SdlCompilationUnit *topLevel) {
	//m_homePath = rootDir;
	//m_moduleName = filename;
	
	std::ifstream inFile(filename.toString());
	if (!inFile.good()) {
		return;
	}

	parseHelper(inFile);
}

void manta::SdlCompilationUnit::parse(const char *sdl, SdlCompilationUnit *topLevel) {
	std::istringstream sdlStream(sdl);
	parse(sdlStream);
}

void manta::SdlCompilationUnit::parse(std::istream &stream, SdlCompilationUnit *topLevel) {
	if (!stream.good() && stream.eof()) {
		return;
	}

	parseHelper(stream);
}

void manta::SdlCompilationUnit::parseHelper(std::istream &stream, SdlCompilationUnit *topLevel) {
	delete m_scanner;
	try {
		m_scanner = new manta::SdlScanner(&stream);
	}
	catch (std::bad_alloc) {
		return;
	}

	delete m_parser;

	try {
		m_parser = new manta::SdlParser(*m_scanner, *this);
	}
	catch (std::bad_alloc) {
		return;
	}

	const int success = 0;
	if (m_parser->parse() != success) {
		std::cerr << "Parse failed!!" << std::endl;
	}
}

void manta::SdlCompilationUnit::addNode(SdlNode *node) {
	m_nodes.push_back(node);
}

manta::SdlNode *manta::SdlCompilationUnit::getNode(int index) const {
	return m_nodes[index];
}

int manta::SdlCompilationUnit::getNodeCount() const {
	return (int)m_nodes.size();
}

void manta::SdlCompilationUnit::addImportStatement(SdlImportStatement *statement) {
	m_importStatements.push_back(statement);
}

int manta::SdlCompilationUnit::getImportStatementCount() const {
	return (int)m_importStatements.size();
}

void manta::SdlCompilationUnit::addNodeDefinition(SdlNodeDefinition *nodeDefinition) {
	m_nodeDefinitions.push_back(nodeDefinition);
}

int manta::SdlCompilationUnit::getNodeDefinitionCount() const {
	return (int)m_nodeDefinitions.size();
}

void manta::SdlCompilationUnit::addCompilationError(SdlCompilationError *err) {
	m_compilationErrors.push_back(err);
}

std::ostream& manta::SdlCompilationUnit::print(std::ostream &stream) {
	return stream;
}
