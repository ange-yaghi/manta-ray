#include "../include/compiler.h"

#include "../include/configuration.h"

#include "../../include/path.h"
#include "../../include/os_utilities.h"

#include <iostream>

mantaray_cli::Compiler::Compiler() {
    m_compiler = new piranha::Compiler(&m_rules);

    m_optimizationEnabled = false;
}

mantaray_cli::Compiler::~Compiler() {
    delete m_compiler;
}

void mantaray_cli::Compiler::initialize() {
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

void mantaray_cli::Compiler::compile(const piranha::IrPath &path) {
    piranha::IrCompilationUnit *unit = m_compiler->compile(path);
    if (unit == nullptr) {
        setState(COULD_NOT_FIND_FILE);
    }
    else {
        const piranha::ErrorList *errors = m_compiler->getErrorList();
        if (errors->getErrorCount() == 0) {
            unit->build(&m_program);
            setState(COMPILATION_SUCCESS);

            if (isOptimizationEnabled()) {
                m_program.initialize();
                m_program.optimize();
                setState(OPTIMIZATION_SUCCESS);
            }
            else {
                setState(OPTIMIZATION_SUCCESS);
            }
        }
        else {
            setState(COMPILATION_FAIL);
        }
    }
}

void mantaray_cli::Compiler::printTrace() {
    if (getState() == COULD_NOT_FIND_FILE) {
        std::cout << "Compilation failed: File not found.\n";
    }
    else if (getState() == COMPILATION_FAIL) {
        const piranha::ErrorList *errors = m_compiler->getErrorList();
        int errorCount = errors->getErrorCount();

        std::cout << "Compilation failed: " << errorCount << " error(s) found\n";
        
        for (int i = 0; i < errorCount; i++) {
            printError(errors->getCompilationError(i));
        }
    }
}

void mantaray_cli::Compiler::execute() {
    if (getState() == OPTIMIZATION_SUCCESS) {
        const bool result = m_program.execute();
        setState(COMPLETE);

        if (!result) {
            std::cout << "Runtime error: " << m_program.getRuntimeError() << std::endl;
        }
    }
}

void mantaray_cli::Compiler::printError(const piranha::CompilationError *err) {
    const piranha::ErrorCode_struct &errorCode = err->getErrorCode();
    std::cout << err->getCompilationUnit()->getPath().getStem()
        << "(" << err->getErrorLocation()->lineStart << "): error "
        << errorCode.stage << errorCode.code << ": " << errorCode.info << std::endl;

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

            std::cout
                << "       While instantiating: "
                << instance->getParentUnit()->getPath().getStem()
                << "(" << instance->getSummaryToken()->lineStart << "): "
                << formattedName << std::endl;
        }

        context = context->getParent();
    }
}
