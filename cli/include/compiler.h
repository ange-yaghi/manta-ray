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

    protected:
        manta::LanguageRules m_rules;
        piranha::Compiler *m_compiler;
        piranha::NodeProgram m_program;

        void setState(STATE state) { m_state = state; }
        STATE m_state;

    protected:
        void printError(const piranha::CompilationError *err);
    };

} /* namespace mantaray_cli */

#endif /* MANTARAY_CLI_COMPILER_H */
