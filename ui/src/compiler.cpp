#include "../include/compiler.h"

#include "../include/configuration.h"
#include "../include/application.h"

#include "../../include/path.h"
#include "../../include/os_utilities.h"
#include "../../include/console.h"

#include <iostream>

mantaray_ui::Compiler::Compiler() {
    m_compiler = new piranha::Compiler(&m_rules);

    m_optimizationEnabled = true;
    m_state = State::Ready;

    m_executionThread = nullptr;
}

mantaray_ui::Compiler::~Compiler() {
    /* void */
}

void mantaray_ui::Compiler::initialize() {
    m_compiler->setFileExtension(".mr");

    manta::Path directory = manta::getModuleDirectory();
    manta::Path confFile = directory.append(manta::Path("mantaray_cli.conf"));

    std::ifstream configurationFile(confFile.toString());
    if (!configurationFile.is_open()) {
        std::cout << "[NON-PRODUCTION VERSION] - mantaray_cli.conf not found" << std::endl;
        m_compiler->addSearchPath(MANTARAY_STANDARD_LIB_DIR);
    }
    else {
        std::string path;
        std::getline(configurationFile, path);

        manta::Path searchDirectory = directory.append(manta::Path(path));

        m_compiler->addSearchPath(searchDirectory.toString());
    }

    m_rules.initialize();
}

void mantaray_ui::Compiler::compile(const piranha::IrPath &path) {
    piranha::IrCompilationUnit *unit = m_compiler->compile(path);
    if (unit == nullptr) {
        setState(State::CouldNotFindFile);
    }
    else {
        const piranha::ErrorList *errors = m_compiler->getErrorList();
        if (errors->getErrorCount() == 0) {
            unit->build(&m_program);
            setState(State::CompilationSuccess);

            if (isOptimizationEnabled()) {
                m_program.initialize();
                m_program.optimize();
                setState(State::OptimizationSuccess);
            }
            else {
                setState(State::OptimizationSuccess);
            }
        }
        else {
            setState(State::CompilationFail);
            printTrace();
        }
    }
}

void mantaray_ui::Compiler::printTrace() {
    if (getState() == State::CouldNotFindFile) {
        std::cout << "Compilation failed: File not found.\n";
    }
    else if (getState() == State::CompilationFail) {
        const piranha::ErrorList *errors = m_compiler->getErrorList();
        int errorCount = errors->getErrorCount();

        std::cout << "Compilation failed: " << errorCount << " error(s) found\n";

        for (int i = 0; i < errorCount; i++) {
            printError(errors->getCompilationError(i));
        }
    }
}

void mantaray_ui::Compiler::execute() {
    if (getState() == State::OptimizationSuccess) {
        const bool result = m_program.execute();
        setState(State::Complete);

        if (!result) {
            std::cout << "Runtime error: " << m_program.getRuntimeError() << std::endl;
        }
    }
}

void mantaray_ui::Compiler::destroy() {
    m_program.free();
    m_compiler->free();
    delete m_compiler;
}

void mantaray_ui::Compiler::kill() {
    m_program.kill();
}

void mantaray_ui::Compiler::printError(const piranha::CompilationError *err) {
    const piranha::ErrorCode_struct &errorCode = err->getErrorCode();
    std::stringstream ss;
    ss << err->getCompilationUnit()->getPath().getStem()
        << "(" << err->getErrorLocation()->lineStart << "): error "
        << errorCode.stage << errorCode.code << ": " << errorCode.info << std::endl;
    manta::Session::get().getConsole()->out(ss.str(), Application::StandardRed);

    piranha::IrContextTree *context = err->getInstantiation();
    while (context != nullptr) {
        piranha::IrNode *instance = context->getContext();
        if (instance != nullptr) {
            std::string instanceName = instance->getName();
            std::string definitionName = (instance->getDefinition() != nullptr)
                ? instance->getDefinition()->getName()
                : std::string("<Type Error>");
            std::string formattedName;
            if (instanceName.empty()) formattedName = "<unnamed> " + definitionName;
            else formattedName = instanceName + " " + definitionName;

            ss = std::stringstream();
            ss
                << "       While instantiating: "
                << instance->getParentUnit()->getPath().getStem()
                << "(" << instance->getSummaryToken()->lineStart << "): "
                << formattedName << std::endl;
            manta::Session::get().getConsole()->out(ss.str(), Application::StandardRed);
        }

        context = context->getParent();
    }
}
