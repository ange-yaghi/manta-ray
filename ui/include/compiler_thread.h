#ifndef MANTARAY_UI_COMPILER_THREAD_H
#define MANTARAY_UI_COMPILER_THREAD_H

#include "compiler.h"

#include <thread>

namespace mantaray_ui {

    class CompilerThread {
    public:
        CompilerThread();
        ~CompilerThread();

        void initialize();
        void compileAndExecute(const piranha::IrPath &path);
        void kill();
        void destroy();

        bool isDone();

    protected:
        void compileAndExecuteThread(const piranha::IrPath &path);

    protected:
        Compiler m_compiler;
        std::atomic<bool> m_done;

        std::thread *m_executionThread;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_COMPILER_THREAD_H */
