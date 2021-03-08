#ifndef DELTA_BASIC_CONSOLE_H
#define DELTA_BASIC_CONSOLE_H

#include "delta_core.h"

#include "font.h"
#include "shader_controls.h"
#include "ui_renderer.h"
#include "console_shaders.h"

#include <string>

namespace dbasic {

    // Class name declarations
    class DeltaEngine;

    class Console : public ysObject {
    public:
        static const int BufferWidth = 175;
        static const int BufferHeight = 75;
        static const int BufferSize = BufferWidth * BufferHeight; // Maximum of 4096 characters displayed at once

    public:
        Console();
        ~Console();

        ysError Initialize();
        ysError ResetScreenPosition();
        ysError Destroy();

        ysError UpdateGeometry();

        void SetDefaultFontDirectory(const std::string &s) { m_defaultFontDirectory = s; }
        std::string GetDefaultFontDirectory() const { return m_defaultFontDirectory; }

        void SetEngine(DeltaEngine *engine) { m_engine = engine; }
        DeltaEngine *GetEngine() const { return m_engine; }

        void SetRenderer(UiRenderer *renderer) { m_renderer = renderer; }
        UiRenderer *GetRenderer() const { return m_renderer; }

        Font *GetFont() const { return m_font; }

    protected:
        // Settings
        std::string m_defaultFontDirectory;

    protected:
        // Window metrics
        int m_bufferWidth;
        int m_bufferHeight;

    protected:
        // ----------------------------------------------------
        // GUI Drawing Tools
        //
        // SYSTEM
        //
        // ----------------------------------------------------

        void RealignLocation() { m_actualLocation = m_nominalLocation; }

        GuiPoint m_nominalLocation;
        GuiPoint m_actualLocation;

        DeltaEngine *m_engine;
        UiRenderer *m_renderer;

        Font *m_font;

        char *m_buffer;

    public:
        // ----------------------------------------------------
        // GUI Drawing Tools
        //
        // INTERFACE
        //
        // ----------------------------------------------------

        // Drawing Text
        ysError SetCharacter(char character);

        void Clear();

        void OutputChar(unsigned char c, int n = 1);
        void DrawGeneralText(const char *text, int maxLength = -1);
        void DrawBoundText(const char *text, int width, int height, int xOffset, int yOffset);
        void DrawWrappedText(const char *text, int width);

        int GetTotalNotWhitespace() const;

        // Drawing Shapes
        void DrawLineRectangle(int width, int height);
        void DrawHorizontalLine(int length);
        void DrawVerticalLine(int length);

        // Utilities
        static int FindEndOfNextWord(const char *text, int location);
        static bool IsWhiteSpace(char c);

        // Navigation
        void MoveDownLine(int n = 1);
        void MoveToLocation(const GuiPoint &location);
        void MoveToOrigin();
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_CONSOLE_H */
