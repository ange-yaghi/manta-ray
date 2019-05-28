#ifndef SDL_DRIVER_H
#define SDL_DRIVER_H

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

namespace manta {

	class SdlParser;
	class SdlScanner;
	class SdlNode;
	class SdlAttributeList;
	class SdlAttribute;
	class SdlValue;

	class SdlDriver {
	public:
		SdlDriver() = default;
		virtual ~SdlDriver();

		void parseFile(const char *filename);
		void parse(const char *sdl);
		void parse(std::istream &iss);

		void addUpper();
		void addLower();
		void addWord(const std::string &word);
		void addNewline();
		void addChar();

		SdlNode *createNode(const std::string &type, const std::string &name, SdlAttributeList *attributes) const;
		void addNode(SdlNode *node);
		SdlNode *getNode(int index) const;
		int getNodeCount() const;

		SdlAttributeList *createAttributeList() const;
		SdlAttribute *createAttribute(const std::string &name, SdlValue *value) const;

		std::ostream& print(std::ostream &stream);

	private:
		void parseHelper(std::istream &stream);

		std::size_t m_chars = 0;
		std::size_t m_words = 0;
		std::size_t m_lines = 0;
		std::size_t m_uppercase = 0;
		std::size_t m_lowercase = 0;
		std::size_t m_nodeCount = 0;
		SdlParser *m_parser = nullptr;
		SdlScanner *m_scanner = nullptr;

		std::vector<SdlNode *> m_nodes;
	};

} /* namespace manta */

#endif /* SDL_DRIVER_H */
