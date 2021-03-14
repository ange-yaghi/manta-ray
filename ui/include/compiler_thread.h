#ifndef MANTARAY_UI_COMPILER_THREAD_H
#define MANTARAY_UI_COMPILER_THREAD_H

#include "compiler.h"

#include <thread>
#include <mutex>
#include <condition_variable>

namespace mantaray_ui {

    class CompilerThread {
    public:
        CompilerThread();
        ~CompilerThread();

        void initialize();
        void compileAndExecute(const piranha::IrPath &path);
        void kill();
        void destroy();

        bool isDestroyed() const { return m_destroyed; }
        bool isExecutionComplete() const { return m_executionComplete; }

        void setContinue(bool cont);
        bool getContinue() const { return m_continue; }

    protected:
        void compileAndExecuteThread(const piranha::IrPath &path);

    protected:
        Compiler m_compiler;
        std::atomic<bool> m_destroyed;
        std::atomic<bool> m_executionComplete;
        bool m_continue;

        std::mutex m_waitLock;
        std::condition_variable m_waitCv;

        std::thread *m_executionThread;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_COMPILER_THREAD_H */
