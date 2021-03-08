#include "../include/compiler.h"

#include "../../include/manta.h"
#include "../../include/session.h"
#include "../../include/console.h"

#include <iostream>

#define VERSION "v1.2.0a"

void printHeader() {
    std::cout << "////////////////////////////////////////////////" << std::endl;
    std::cout << std::endl;
    std::cout << "  MantaRay CLI                          " VERSION << std::endl;
    std::cout << "" << std::endl;
    std::cout << "////////////////////////////////////////////////" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "  Author  | Ange Yaghi [me@angeyaghi.com]" << std::endl;
    std::cout << "  Date    | " __DATE__ << std::endl;
    std::cout << "  Project | github.com/ange-yaghi/manta-ray " << std::endl;
    std::cout << "  Piranha | " PIRANHA_VERSION << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
}

int main(int argc, char *argv[]) {
    printHeader();

    mantaray_cli::Compiler compiler;
    compiler.initialize();

    manta::Session::get().setConsole(new manta::Console());

    std::string scriptName;
    if (argc == 2) {
        scriptName = argv[1];

        std::cout << "  Script: " << scriptName << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
    }
    else if (argc == 1) {
        std::cout << "  Script: ";
        std::getline(std::cin, scriptName);

        std::cout << "------------------------------------------------" << std::endl;
    }

    std::cout << "  Compiling..." << std::endl;
    compiler.compile(scriptName);
    compiler.printTrace();

    std::cout << "------------------------------------------------" << std::endl;
    compiler.execute();
    
    std::string cmd;
    std::cout << "Exit? ";
    std::cin.ignore();

    return 0;
}
