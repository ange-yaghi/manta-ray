#ifndef PIRANHA_IR_COMPILATION_UNIT_H
#define PIRANHA_IR_COMPILATION_UNIT_H

#include "ir_parser_structure.h"

#include "path.h"

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

namespace piranha {

	class IrParser;
	class IrScanner;
	class IrNode;
	class IrNodeDefinition;
	class IrAttributeList;
	class IrAttribute;
	class IrValue;
	class IrImportStatement;
	class IrCompilationError;
	class IrErrorList;
	class NodeProgram;

	class IrCompilationUnit : public IrParserStructure {
	public:
		enum ParseResult {
			IO_ERROR,
			SUCCESS,
			FAIL
		};

	public:
		IrCompilationUnit() = default;
		virtual ~IrCompilationUnit();

		void build(NodeProgram *program);

		ParseResult parseFile(const Path &filename, IrCompilationUnit *topLevel = nullptr);
		ParseResult parse(const char *sdl, IrCompilationUnit *topLevel = nullptr);
		ParseResult parse(std::istream &iss, IrCompilationUnit *topLevel = nullptr);

		void addNode(IrNode *node);
		IrNode *getNode(int index) const;
		int getNodeCount() const;

		void addImportStatement(IrImportStatement *statement);
		IrImportStatement *getImportStatement(int index) const { return m_importStatements[index]; }
		int getImportStatementCount() const;

		void addNodeDefinition(IrNodeDefinition *definition);
		IrNodeDefinition *getNodeDefinition(int index) const { return m_nodeDefinitions[index]; }
		int getNodeDefinitionCount() const;

		virtual IrParserStructure *resolveLocalName(const std::string &name) const;
		int countSymbolIncidence(const std::string &name) const;

		void addCompilationError(IrCompilationError *err);

		std::ostream& print(std::ostream &stream);

		void addDependency(IrCompilationUnit *unit) { m_dependencies.push_back(unit); registerComponent(unit); }
		IrCompilationUnit *getDependency(int index) { return m_dependencies[index]; }
		int getDependencyCount() const { return (int)m_dependencies.size(); }
		const Path &getPath() const { return m_path; }

		void setErrorList(IrErrorList *list) { m_errorList = list; }
		IrErrorList *getErrorList() const { return m_errorList; }

	private:
		ParseResult parseHelper(std::istream &stream, IrCompilationUnit *topLevel = nullptr);

		IrParser *m_parser = nullptr;
		IrScanner *m_scanner = nullptr;

		Path m_path;

		std::vector<IrNode *> m_nodes;
		std::vector<IrImportStatement *> m_importStatements;
		std::vector<IrNodeDefinition *> m_nodeDefinitions;
		std::vector<IrCompilationUnit *> m_dependencies;

		IrErrorList *m_errorList = nullptr;

		// Resolution stage
	public:
		IrNodeDefinition *resolveLocalNodeDefinition(const std::string &name, int *count, bool external = false);
		IrNodeDefinition *resolveNodeDefinition(const std::string &name, int *count,
			const std::string &libraryName, bool external = false);

		// Validation stage
	protected:
		virtual void _validate();
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_COMPILATION_UNIT_H */
