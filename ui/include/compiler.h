#ifndef MANTARAY_CLI_COMPILER_H
#define MANTARAY_CLI_COMPILER_H

#include "../../include/language_rules.h"

#include <piranha.h>

#include <thread>

namespace mantaray_ui {

    class Compiler {
    protected:
        enum class State {
            Ready,
            CompilationSuccess,
            CompilationFail,
            OptimizationSuccess,
            OptimizationFail,
            CouldNotFindFile,
            Complete
        };

    public:
        Compiler();
        ~Compiler();

        void initialize();

        void compile(const piranha::IrPath &path);
        void printTrace();
        void execute();
        void destroy();

        State getState() const { return m_state; }

        void setOptimizationEnabled(bool enabled) { m_optimizationEnabled = enabled; }
        bool isOptimizationEnabled() const { return m_optimizationEnabled; }

        void kill();

    protected:
        manta::LanguageRules m_rules;
        piranha::Compiler *m_compiler;
        piranha::NodeProgram m_program;

        bool m_optimizationEnabled;

        void setState(State state) { m_state = state; }
        State m_state;

        std::thread *m_executionThread;

    protected:
        void printError(const piranha::CompilationError *err);
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_COMPILER_H */
