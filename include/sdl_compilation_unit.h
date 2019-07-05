#ifndef SDL_COMPILATION_UNIT_H
#define SDL_COMPILATION_UNIT_H

#include <sdl_parser_structure.h>

#include <path.h>

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
	class SdlErrorList;

	class SdlCompilationUnit : public SdlParserStructure {
	public:
		enum ParseResult {
			IO_ERROR,
			SUCCESS,
			FAIL
		};

	public:
		SdlCompilationUnit() = default;
		virtual ~SdlCompilationUnit();

		ParseResult parseFile(const Path &filename, SdlCompilationUnit *topLevel = nullptr);
		ParseResult parse(const char *sdl, SdlCompilationUnit *topLevel = nullptr);
		ParseResult parse(std::istream &iss, SdlCompilationUnit *topLevel = nullptr);

		void addNode(SdlNode *node);
		SdlNode *getNode(int index) const;
		int getNodeCount() const;

		void addImportStatement(SdlImportStatement *statement);
		SdlImportStatement *getImportStatement(int index) const { return m_importStatements[index]; }
		int getImportStatementCount() const;

		void addNodeDefinition(SdlNodeDefinition *definition);
		SdlNodeDefinition *getNodeDefinition(int index) const { return m_nodeDefinitions[index]; }
		int getNodeDefinitionCount() const;

		virtual SdlParserStructure *resolveLocalName(const std::string &name) const;
		int countSymbolIncidence(const std::string &name) const;

		void addCompilationError(SdlCompilationError *err);

		std::ostream& print(std::ostream &stream);

		void addDependency(SdlCompilationUnit *unit) { m_dependencies.push_back(unit); registerComponent(unit); }
		SdlCompilationUnit *getDependency(int index) { return m_dependencies[index]; }
		int getDependencyCount() const { return (int)m_dependencies.size(); }
		const Path &getPath() const { return m_path; }

		void setErrorList(SdlErrorList *list) { m_errorList = list; }
		SdlErrorList *getErrorList() const { return m_errorList; }

	private:
		ParseResult parseHelper(std::istream &stream, SdlCompilationUnit *topLevel = nullptr);

		SdlParser *m_parser = nullptr;
		SdlScanner *m_scanner = nullptr;

		Path m_path;

		std::vector<SdlNode *> m_nodes;
		std::vector<SdlImportStatement *> m_importStatements;
		std::vector<SdlNodeDefinition *> m_nodeDefinitions;
		std::vector<SdlCompilationUnit *> m_dependencies;

		SdlErrorList *m_errorList = nullptr;

		// Resolution stage
	public:
		SdlNodeDefinition *resolveLocalNodeDefinition(const std::string &name, int *count, bool external = false);
		SdlNodeDefinition *resolveNodeDefinition(const std::string &name, int *count,
			const std::string &libraryName, bool external = false);

		// Validation stage
	protected:
		virtual void _validate();
	};

} /* namespace manta */

#endif /* SDL_COMPILATION_UNIT_H */
