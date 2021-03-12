#include "../include/ui_console.h"

#include "../include/application.h"
#include "../../include/color.h"

#include <sstream>

mantaray_ui::UiConsole::UiConsole() {
    m_textRenderer = nullptr;
    m_atLineBegin = true;
}

mantaray_ui::UiConsole::~UiConsole() {
    /* void */
}

void mantaray_ui::UiConsole::initialize(dbasic::TextRenderer *textRenderer, dbasic::Font *font) {
    m_textRenderer = textRenderer;
    m_textRenderer->SetFont(font);

    m_lines.push_back({});

    out("///////////////// MantaRay ////////////////\n", ysColor::srgbiToSrgb(0x00A4E9));
    out("*                                         *\n", ysColor::srgbiToSrgb(0x00A4E9));
    out("* 2021 | (c) [A|T|G]                      *\n", ysColor::srgbiToSrgb(0x00A4E9));
    out("*                                         *\n", ysColor::srgbiToSrgb(0x00A4E9));
    out("///////////////////////////////////////////\n", ysColor::srgbiToSrgb(0x00A4E9));
    
    out("Listening...\n", ysColor::srgbiToSrgb(0xFF9F15));
}

void mantaray_ui::UiConsole::setExtents(const BoundingBox &box) {
    m_boundingBox = box;
}

void mantaray_ui::UiConsole::out(const std::string &text, const manta::math::Vector &color) {
    std::lock_guard<std::mutex> lock(m_outputLock);

    for (const char &c : text) {
        if (c == '\n') {
            m_lines.push_back({});            
            m_atLineBegin = true;
        }
        else if (c == '\r') {
            m_atLineBegin = true;
        }
        else {
            if (m_atLineBegin) {
                m_lines.back().textBlocks.clear();
                m_lines.back().textBlocks.push_back({ "", color });

                m_atLineBegin = false;
            }

            std::string &line = m_lines.back().textBlocks.back().text;
            line += std::string({ c });
        }
    }
}

void mantaray_ui::UiConsole::outCentered(const std::string &text, const manta::math::Vector &color) {
    std::lock_guard<std::mutex> lock(m_outputLock);

    if (!m_atLineBegin) {
        m_lines.push_back({});
    }

    m_lines.back().centered = true;
    m_lines.back().textBlocks = { { text, color } };
    m_lines.push_back({});

    m_atLineBegin = true;
}

void mantaray_ui::UiConsole::render() {
    std::lock_guard<std::mutex> lock(m_outputLock);

    const int w = m_textRenderer->GetEngine()->GetScreenWidth();
    const int h = m_textRenderer->GetEngine()->GetScreenHeight();

    const float height = 16;

    int currentLine = 0;
    for (const TextLine &line : m_lines) {
        for (const TextBlock &block : line.textBlocks) {
            m_textRenderer->SetColor(block.color);
            
            float offset = 0.0f;
            if (line.centered) {
                const float width = m_textRenderer->CalculateWidth(block.text, height);
                offset = -width / 2.0f + m_boundingBox.Width() / 2.0f;
            }

            m_textRenderer->RenderText(
                block.text,
                m_boundingBox.Left() - (w / 2.0f) + offset,
                m_boundingBox.Top() - (currentLine + 1) * height - (h / 2.0f),
                height
            );
        }

        ++currentLine;
    }
}
