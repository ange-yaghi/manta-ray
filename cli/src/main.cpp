#include "../../include/manta.h"
#include "../include/compiler.h"

#include <iostream>

int main(int argc, char *argv[]) {
    mantaray_cli::Compiler compiler;
    compiler.initialize();

    std::string scriptName;
    if (argc == 2) {
        scriptName = argv[1];
    }
    else if (argc == 1) {
        std::cout << "Script name: ";
        std::cin >> scriptName;
    }

    compiler.compile(scriptName);
    compiler.printTrace();
    compiler.execute();
}
