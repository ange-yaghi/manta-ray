#include "demos.h"

#include "../include/path.h"
#include "../include/language_rules.h"

using namespace manta;

void manta_demo::sdlDemo(int samplesPerPixel, int resolutionX, int resolutionY) {
    LanguageRules *rules = new LanguageRules();
    rules->registerBuiltinNodeTypes();

    piranha::Compiler *compiler = new piranha::Compiler(rules);
    compiler->addSearchPath(SDL_LIB_PATH);
    piranha::IrCompilationUnit *unit = compiler->compile(SDL_PATH + std::string("blocks_demo.mr"));

    const piranha::ErrorList *errList = compiler->getErrorList();

    if (errList->getErrorCount() > 0) {
        std::cout << "Found " << errList->getErrorCount() << " compilation errors\n";
        return;
    }

    piranha::NodeProgram program;
    unit->build(&program);

    program.execute();
}
