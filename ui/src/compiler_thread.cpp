#include "../include/compiler_thread.h"

mantaray_ui::CompilerThread::CompilerThread() {
    m_executionThread = nullptr;
    m_done = false;
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

void mantaray_ui::CompilerThread::compileAndExecuteThread(const piranha::IrPath &path) {
    m_compiler.compile(path);
    m_compiler.execute();
    m_compiler.destroy();

    m_done = true;
}
