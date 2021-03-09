#ifndef MANTARAY_UI_CONSOLE_H
#define MANTARAY_UI_CONSOLE_H

#include "../../include/console.h"
#include "../include/bounding_box.h"

#include "delta.h"

#include <vector>
#include <mutex>

namespace mantaray_ui {

    class UiConsole : public manta::Console {
    public:
        struct TextBlock {
            std::string text;
            ysVector color;
        };

        struct TextLine {
            std::vector<TextBlock> textBlocks;
            bool centered = false;
        };

    public:
        UiConsole();
        ~UiConsole();

        void initialize(dbasic::TextRenderer *textRenderer, dbasic::Font *font);
        void setExtents(const BoundingBox &box);

        virtual void out(const std::string &text, const manta::math::Vector &color = manta::math::constants::One);

        void outCentered(const std::string &text, const manta::math::Vector &color = manta::math::constants::One);

        void render();

    protected:
        std::vector<TextLine> m_lines;
        dbasic::TextRenderer *m_textRenderer;

        std::mutex m_outputLock;

        bool m_atLineBegin;

        BoundingBox m_boundingBox;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_CONSOLE_H */
