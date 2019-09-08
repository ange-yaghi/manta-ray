#ifndef MANTARAY_CLI_COMPILER_H
#define MANTARAY_CLI_COMPILER_H

#include "../../include/language_rules.h"

#include <piranha.h>

namespace mantaray_cli {

    class Compiler {
    protected:
        enum STATE {
            READY,
            COMPILATION_SUCCESS,
            COMPILATION_FAIL,
            OPTIMIZATION_SUCCESS,
            OPTIMIZATION_FAIL,
            COULD_NOT_FIND_FILE,
            COMPLETE
        };

    public:
        Compiler();
        ~Compiler();

        void initialize();

        void compile(const piranha::IrPath &path);
        void printTrace();
        void execute();

        STATE getState() const { return m_state; }

        void setOptimizationEnabled(bool enabled) { m_optimizationEnabled = enabled; }
        bool isOptimizationEnabled() const { return m_optimizationEnabled; }

    protected:
        manta::LanguageRules m_rules;
        piranha::Compiler *m_compiler;
        piranha::NodeProgram m_program;

        bool m_optimizationEnabled;

        void setState(STATE state) { m_state = state; }
        STATE m_state;

    protected:
        void printError(const piranha::CompilationError *err);
    };

} /* namespace mantaray_cli */

#endif /* MANTARAY_CLI_COMPILER_H */
