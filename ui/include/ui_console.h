#ifndef MANTARAY_UI_CONSOLE_H
#define MANTARAY_UI_CONSOLE_H

#include "../../include/console.h"

#include "delta.h"

#include <vector>

namespace mantaray_ui {

    class UiConsole : public manta::Console {
    public:
        UiConsole();
        ~UiConsole();

        void initialize(dbasic::TextRenderer *textRenderer);

        virtual void out(const std::string &text);

        void render();

    protected:
        std::vector<std::string> m_lines;
        dbasic::TextRenderer *m_textRenderer;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_CONSOLE_H */
