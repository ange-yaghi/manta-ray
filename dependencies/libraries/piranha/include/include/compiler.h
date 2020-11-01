#ifndef PIRANHA_COMPILER_H
#define PIRANHA_COMPILER_H

#include "path.h"
#include "error_list.h"

#include <vector>
#include <string>

namespace piranha {

    class IrCompilationUnit;
    class LanguageRules;

    typedef Path IrPath;

    class Compiler {
    public:
        Compiler(const LanguageRules *rules = nullptr);
        ~Compiler();

        IrCompilationUnit *compile(const IrPath &scriptPath);
        void free();
        IrCompilationUnit *getUnit(const IrPath &scriptPath) const;

        int getUnitCount() const { return (int)m_units.size(); }

        const ErrorList *getErrorList() const { return &m_errorList; }

        void setFileExtension(const std::string &extension) { m_extension = extension; }
        std::string getFileExtension() const { return m_extension; }

        void addSearchPath(const IrPath &path);
        int getSearchPathCount() const { return (int)m_searchPaths.size(); }

        bool resolvePath(const IrPath &path, IrPath *target) const;

    protected:
        IrCompilationUnit *analyze(const IrPath &scriptPath);
        bool isPathEquivalent(const IrPath &a, const IrPath &b) const;

        static bool hasEnding(std::string const &fullString, std::string const &ending);

    protected:
        // Build steps
        void resolve();
        void validate();

    protected:
        const LanguageRules *m_rules;

        ErrorList m_errorList;
        std::vector<IrCompilationUnit *> m_units;

        std::vector<IrPath> m_searchPaths;
        std::string m_extension;
    };

} /* namespace piranha */

#endif /* PIRANHA_COMPILER_H */
