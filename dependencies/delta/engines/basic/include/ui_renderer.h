#ifndef DELTA_BASIC_UI_RENDERER_H
#define DELTA_BASIC_UI_RENDERER_H

#include "delta_core.h"

#include "shader_controls.h"
#include "font.h"
#include "console_shaders.h"

namespace dbasic {

    class DeltaEngine;

    struct GuiPoint {
        GuiPoint(int x, int y) : x(x), y(y) {}
        GuiPoint() : x(0), y(0) {}
        ~GuiPoint() {}

        GuiPoint operator+(const GuiPoint &p) {
            return GuiPoint(x + p.x, y + p.y);
        }

        GuiPoint operator-(const GuiPoint &p) {
            return GuiPoint(x - p.x, y - p.y);
        }

        GuiPoint operator+=(const GuiPoint &p) {
            x += p.x;
            y += p.y;

            return *this;
        }

        GuiPoint operator-=(const GuiPoint &p) {
            x -= p.x;
            y -= p.y;

            return *this;
        }

        void Clear() {
            x = 0;
            y = 0;
        }

        int x;
        int y;
    };

    class UiRenderer : public ysObject {
    public:
        UiRenderer();
        ~UiRenderer();

        ysError Initialize(int bufferSize);
        ysError Reset();
        ysError UpdateDisplay();
        ysError Destroy();

        void SetEngine(DeltaEngine *engine) { m_engine = engine; }
        DeltaEngine *GetEngine() { return m_engine; }

        void SetFont(Font *font) { m_font = font; }
        Font *GetFont() const { return m_font; }

        ConsoleVertex *AllocateQuads(int n);
        ConsoleVertex *AllocateTriangles(int n);  

        ConsoleShaders *GetShaders() { return &m_shaders; }

    protected:
        ysError InitializeGeometry(int bufferSize);

    protected:
        DeltaEngine *m_engine;
        ConsoleShaders m_shaders;

        ysGPUBuffer *m_mainVertexBuffer;
        ysGPUBuffer *m_mainIndexBuffer;

        int m_bufferSize;
        int m_vertexOffset;
        int m_indexOffset;

        // Vertex buffer
        ConsoleVertex *m_vertexBuffer;
        unsigned short *m_indexBuffer;

        Font *m_font;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_UI_RENDERER_H */
