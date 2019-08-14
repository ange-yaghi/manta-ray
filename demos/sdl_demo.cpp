#include "demos.h"

#include "../include/path.h"
#include "../include/language_rules.h"

using namespace manta;

void printError(const piranha::CompilationError *err) {
    const piranha::ErrorCode_struct &errorCode = err->getErrorCode();
    std::cout << err->getCompilationUnit()->getPath().getStem() << "(" << err->getErrorLocation()->lineStart << "): error " << errorCode.stage << errorCode.code << ": " << errorCode.info << std::endl;

    piranha::IrContextTree *context = err->getInstantiation();
    while (context != nullptr) {
        piranha::IrNode *instance = context->getContext();
        if (instance != nullptr) {
            std::string instanceName = instance->getName();
            std::string definitionName = (instance->getDefinition() != nullptr) ? instance->getDefinition()->getName() : std::string("<Type Error>");
            std::string formattedName;
            if (instanceName.empty()) formattedName = "<unnamed> " + definitionName;
            else formattedName = instanceName + " " + definitionName;

            std::cout <<
                "       While instantiating: " <<
                instance->getParentUnit()->getPath().getStem() << "(" << instance->getSummaryToken()->lineStart << "): " << formattedName << std::endl;
        }

        context = context->getParent();
    }
}

void printErrorTrace(const piranha::ErrorList *errList) {
    int errorCount = errList->getErrorCount();
    for (int i = 0; i < errorCount; i++) {
        printError(errList->getCompilationError(i));
    }
}

void manta_demo::sdlDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
    LanguageRules *rules = new LanguageRules();
    rules->registerBuiltinNodeTypes();

    piranha::Compiler *compiler = new piranha::Compiler(rules);
    compiler->addSearchPath(SDL_LIB_PATH);
    piranha::IrCompilationUnit *unit = compiler->compile(SDL_PATH + std::string("pen_demo.mr"));

    const piranha::ErrorList *errList = compiler->getErrorList();

    if (errList->getErrorCount() > 0) {
        printErrorTrace(errList);
        std::cout << "Found " << errList->getErrorCount() << " compilation errors\n";
        return;
    }

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();
}
