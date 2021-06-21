#ifndef DELTA_BASIC_TEXT_RENDERER_H
#define DELTA_BASIC_TEXT_RENDERER_H

#include "font.h"
#include "delta_core.h"
#include "shader_controls.h"
#include "ui_renderer.h"

#include <string>

namespace dbasic {

    // Class name declarations
    class DeltaEngine;

    class TextRenderer : public ysObject {
    public:
        TextRenderer();
        ~TextRenderer();

        void Initialize();

        void SetFont(Font *font) { m_font = font; }
        Font *GetFont() const { return m_font; }

        void SetEngine(DeltaEngine *engine) { m_engine = engine; }
        DeltaEngine *GetEngine() const { return m_engine; }

        void SetRenderer(UiRenderer *renderer) { m_renderer = renderer; }
        UiRenderer *GetRenderer() const { return m_renderer; }

        void SetColor(const ysVector &color) { m_currentColor = color; }
        ysVector GetColor() const { return m_currentColor; }

    protected:
        DeltaEngine *m_engine;
        UiRenderer *m_renderer;

        Font *m_font;

        ysVector m_currentColor;

    public:
        // Drawing Text
        void RenderText(const std::string &s, float x, float y, float h);
        void RenderMonospaceText(const std::string &s, float x, float y, float h, float w);

        float CalculateWidth(const std::string &s, float h) const;

        // Utilities
        static bool IsWhitespace(char c);
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_CONSOLE_H */
