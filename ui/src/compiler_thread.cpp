#include "../include/compiler_thread.h"

mantaray_ui::CompilerThread::CompilerThread() {
    m_executionThread = nullptr;
    m_executionComplete = false;
    m_destroyed = false;
    m_continue = false;
}

mantaray_ui::CompilerThread::~CompilerThread() {
    /* void */
}

void mantaray_ui::CompilerThread::initialize() {
    m_compiler.initialize();
}

void mantaray_ui::CompilerThread::compileAndExecute(const piranha::IrPath &path) {
    m_executionThread = new std::thread(&CompilerThread::compileAndExecuteThread, this, path);
}

void mantaray_ui::CompilerThread::kill() {
    m_compiler.kill();
}

void mantaray_ui::CompilerThread::destroy() {
    /* void */
}

void mantaray_ui::CompilerThread::setContinue(bool cont) {
    std::lock_guard<std::mutex> lock(m_waitLock);
    m_continue = cont;

    m_waitCv.notify_one();
}

void mantaray_ui::CompilerThread::compileAndExecuteThread(const piranha::IrPath &path) {
    m_compiler.compile(path);
    m_compiler.execute();

    m_executionComplete = true;

    std::unique_lock<std::mutex> lock(m_waitLock);
    m_waitCv.wait(lock, [this] { return m_continue; });

    m_compiler.destroy();

    m_destroyed = true;

    lock.unlock();
    m_waitCv.notify_one();
}
