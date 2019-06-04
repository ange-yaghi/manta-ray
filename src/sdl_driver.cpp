#include <sdl_driver.h>

#include <sdl_scanner.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>

#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream>

manta::SdlDriver::~SdlDriver() {
	delete m_scanner;
	m_scanner = nullptr;

	delete m_parser;
	m_parser = nullptr;
}

void manta::SdlDriver::parseFile(const char *const filename) {
	assert(filename != nullptr);
	std::ifstream inFile(filename);
	if (!inFile.good()) {
		return;
	}

	parseHelper(inFile);
}

void manta::SdlDriver::parse(const char *sdl) {
	std::istringstream sdlStream(sdl);
	parse(sdlStream);
}

void manta::SdlDriver::parse(std::istream &stream) {
	if (!stream.good() && stream.eof()) {
		return;
	}

	parseHelper(stream);
}

void manta::SdlDriver::parseHelper(std::istream &stream) {
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

void manta::SdlDriver::addNode(SdlNode *node) {
	m_nodes.push_back(node);
}

manta::SdlNode *manta::SdlDriver::getNode(int index) const {
	return m_nodes[index];
}

int manta::SdlDriver::getNodeCount() const {
	return (int)m_nodes.size();
}

void manta::SdlDriver::addImportStatement(SdlImportStatement *statement) {
	m_importStatements.push_back(statement);
}

int manta::SdlDriver::getImportStatementCount() const {
	return (int)m_importStatements.size();
}

void manta::SdlDriver::addNodeDefinition(SdlNodeDefinition *nodeDefinition) {
	m_nodeDefinitions.push_back(nodeDefinition);
}

int manta::SdlDriver::getNodeDefinitionCount() const {
	return (int)m_nodeDefinitions.size();
}

void manta::SdlDriver::addCompilationError(SdlCompilationError *err) {
	m_compilationErrors.push_back(err);
}

std::ostream& manta::SdlDriver::print(std::ostream &stream) {
	return stream;
}
