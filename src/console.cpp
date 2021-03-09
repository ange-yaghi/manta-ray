#include "../include/console.h"

#include <iostream>

manta::Console::Console() {
    /* void */
}

manta::Console::~Console() {
    /* void */
}

void manta::Console::out(const std::string &text, const math::Vector &color) {
    std::cout << text;
}
