#include "../include/ui_console.h"

#include "../include/application.h"

#include <sstream>

mantaray_ui::UiConsole::UiConsole() {
    /* void */
}

mantaray_ui::UiConsole::~UiConsole() {
    /* void */
}

void mantaray_ui::UiConsole::initialize(dbasic::TextRenderer *textRenderer) {
    m_textRenderer = textRenderer;
}

void mantaray_ui::UiConsole::out(const std::string &text) {
    std::stringstream ss(text);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        m_lines.push_back(line);
    }
}

void mantaray_ui::UiConsole::render() {
    const float height = 24;

    int currentLine = 0;
    for (const std::string &line : m_lines) {
        m_textRenderer->RenderText(
            line,
            0.0f,
            currentLine * height,
            height
        );

        ++currentLine;
    }
}
