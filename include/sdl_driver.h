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
	class SdlNodeDefinition;
	class SdlAttributeList;
	class SdlAttribute;
	class SdlValue;
	class SdlImportStatement;
	class SdlCompilationError;

	class SdlDriver {
	public:
		SdlDriver() = default;
		virtual ~SdlDriver();

		void parseFile(const char *filename);
		void parse(const char *sdl);
		void parse(std::istream &iss);

		void addNode(SdlNode *node);
		SdlNode *getNode(int index) const;
		int getNodeCount() const;

		void addImportStatement(SdlImportStatement *statement);
		SdlImportStatement *getImportStatement(int index) const { return m_importStatements[index]; }
		int getImportStatementCount() const;

		void addNodeDefinition(SdlNodeDefinition *definition);
		SdlNodeDefinition *getNodeDefinition(int index) const { return m_nodeDefinitions[index]; }
		int getNodeDefinitionCount() const;

		void addCompilationError(SdlCompilationError *err);
		SdlCompilationError *getCompilationError(int index) { return m_compilationErrors[index]; }
		int getCompilationErrorCount() const { return (int)m_compilationErrors.size(); }

		std::ostream& print(std::ostream &stream);

	private:
		void parseHelper(std::istream &stream);

		SdlParser *m_parser = nullptr;
		SdlScanner *m_scanner = nullptr;

		std::vector<SdlNode *> m_nodes;
		std::vector<SdlImportStatement *> m_importStatements;
		std::vector<SdlNodeDefinition *> m_nodeDefinitions;
		std::vector<SdlCompilationError *> m_compilationErrors;
	};

} /* namespace manta */

#endif /* SDL_DRIVER_H */
