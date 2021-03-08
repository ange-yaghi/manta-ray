#ifndef DELTA_BASIC_FONTS_H
#define DELTA_BASIC_FONTS_H

#include "delta_core.h"

#include <string>

namespace dbasic {

    class Font {
    public:
        struct GlyphData {
            ysVector2 p0;
            ysVector2 p1;
            ysVector2 uv0;
            ysVector2 uv1;
            float Shift;
        };

    public:
        Font();
        ~Font();

        void Initialize(int firstGlyph, int glyphCount, void *cdata, float fontHeight, ysTexture *texture);

        const GlyphData *GetGlyphData(int glyph) const;

        ysTexture *GetTexture() const { return m_texture;  }
        float GetFontHeight() const { return m_fontHeight; }

    protected:
        float m_fontHeight;
        int m_firstGlyph;
        int m_glyphCount;
        GlyphData *m_glyphData;

        ysTexture *m_texture;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_FONTS_H */
