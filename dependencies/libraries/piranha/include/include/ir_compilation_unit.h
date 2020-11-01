#ifndef PIRANHA_IR_COMPILATION_UNIT_H
#define PIRANHA_IR_COMPILATION_UNIT_H

#include "ir_parser_structure.h"

#include "path.h"

#include <string>
#include <cstddef>
#include <istream>
#include <vector>

namespace piranha {

    class Parser;
    class Scanner;
    class IrNode;
    class IrNodeDefinition;
    class IrAttributeList;
    class IrAttribute;
    class IrValue;
    class IrImportStatement;
    class CompilationError;
    class ErrorList;
    class NodeProgram;

    class IrCompilationUnit : public IrParserStructure {
    public:
        enum class ParseResult {
            IoError,
            Success,
            Fail
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
        IrImportStatement *getImportStatement(int index) const;
        int getImportStatementCount() const;

        void addNodeDefinition(IrNodeDefinition *definition);
        IrNodeDefinition *getNodeDefinition(int index) const;
        int getNodeDefinitionCount() const;

        virtual IrParserStructure *resolveLocalName(const std::string &name) const;
        int countSymbolIncidence(const std::string &name) const;

        void addCompilationError(CompilationError *err);

        std::ostream& print(std::ostream &stream);

        void addDependency(IrCompilationUnit *unit);
        IrCompilationUnit *getDependency(int index) { return m_dependencies[index]; }
        int getDependencyCount() const { return (int)m_dependencies.size(); }
        const Path &getPath() const { return m_path; }

        void setErrorList(ErrorList *list) { m_errorList = list; }
        ErrorList *getErrorList() const { return m_errorList; }

        void resolveAll();
        void validateAll();

        void registerStructure(IrParserStructure *structure);

        virtual void free();

    protected:
        virtual void _expand();
        virtual void _checkInstantiation();
        virtual void _checkTypes();

    private:
        ParseResult parseHelper(std::istream &stream, IrCompilationUnit *topLevel = nullptr);

        Parser *m_parser = nullptr;
        Scanner *m_scanner = nullptr;

        Path m_path;

        std::vector<IrParserStructure *> m_allStructures;

        std::vector<IrNode *> m_nodes;
        std::vector<IrImportStatement *> m_importStatements;
        std::vector<IrNodeDefinition *> m_nodeDefinitions;
        std::vector<IrCompilationUnit *> m_dependencies;

        bool m_unitResolved = false;
        bool m_unitValidated = false;

        ErrorList *m_errorList = nullptr;

        // Resolution stage
    public:
        IrNodeDefinition *resolveLocalNodeDefinition(
            const std::string &name, int *count, bool external = false);
        IrNodeDefinition *resolveNodeDefinition(
            const std::string &name, int *count, 
            const std::string &libraryName, bool external = false);
        IrNodeDefinition *resolveLocalBuiltinNodeDefinition(
            const std::string &builtinName, int *count, bool external = false);
        IrNodeDefinition *resolveBuiltinNodeDefinition(
            const std::string &builtinName, int *count, bool external = false);

        // Validation stage
    protected:
        virtual void _validate();
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_COMPILATION_UNIT_H */
