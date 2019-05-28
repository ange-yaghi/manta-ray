#include <cctype>
#include <fstream>
#include <cassert>
#include <sstream>

#include <sdl_driver.h>
#include <sdl_scanner.h>
#include <sdl_node.h>
#include <sdl_attribute_list.h>
#include <sdl_attribute.h>
#include <sdl_value_label.h>

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
	catch (std::bad_alloc &ba) {
		return;
	}

	delete m_parser;

	try {
		m_parser = new manta::SdlParser(*m_scanner, *this);
	}
	catch (std::bad_alloc &ba) {
		return;
	}

	const int success = 0;
	if (m_parser->parse() != success) {
		std::cerr << "Parse failed!!" << std::endl;
	}
}

void manta::SdlDriver::addUpper() {
	m_uppercase++;
	m_chars++;
	m_words++;
}

void manta::SdlDriver::addLower() {
	m_lowercase++;
	m_chars++;
	m_words++;
}

void manta::SdlDriver::addWord(const std::string &word) {
	m_words++;
	m_chars += word.length();

	for (const char &c : word) {
		if (std::islower(c)) {
			m_lowercase++;
		}
		else if (isupper(c)) {
			m_uppercase++;
		}
	}
}

void manta::SdlDriver::addNewline() {
	m_lines++;
	m_chars++;
}

void manta::SdlDriver::addChar() {
	m_chars++;
}

manta::SdlNode *manta::SdlDriver::createNode(const std::string &type, const std::string &name, SdlAttributeList *attributes) const {
	SdlNode *newNode = new SdlNode(type, name, attributes);
	return newNode;
}

void manta::SdlDriver::addNode(SdlNode *node) {
	m_nodeCount++;
	m_nodes.push_back(node);
}

manta::SdlNode *manta::SdlDriver::getNode(int index) const {
	return m_nodes[index];
}

int manta::SdlDriver::getNodeCount() const {
	return (int)m_nodeCount;
}

manta::SdlAttributeList *manta::SdlDriver::createAttributeList() const {
	return new SdlAttributeList;
}

manta::SdlAttribute *manta::SdlDriver::createAttribute(const std::string &name, SdlValue *value) const {
	return new SdlAttribute(name, value);
}

std::ostream& manta::SdlDriver::print(std::ostream &stream) {
	return stream;
}
