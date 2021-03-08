#ifndef DELTA_BASIC_TEXTURE_ASSET_H
#define DELTA_BASIC_TEXTURE_ASSET_H

#include "delta_core.h"

#include <string>

namespace dbasic {

    class TextureAsset : public ysObject {
    public:
        TextureAsset();
        ~TextureAsset();

        void SetName(const std::string &name) { m_name = name; }
        std::string GetName() const { return m_name; }

        void SetTexture(ysTexture *texture) { m_texture = texture; }
        ysTexture *GetTexture() const { return m_texture; }

        ysError Destroy(ysDevice *device);

    protected:
        ysTexture *m_texture;
        std::string m_name;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_TEXTURE_ASSET_H */
