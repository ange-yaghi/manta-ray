#ifndef DELTA_BASIC_AUDIO_ASSET_H
#define DELTA_BASIC_AUDIO_ASSET_H

#include "delta_core.h"

#include <string>

namespace dbasic {

    class AudioAsset : public ysObject {
    public:
        AudioAsset();
        ~AudioAsset();

        void SetBuffer(ysAudioBuffer *buffer) { m_buffer = buffer; }
        ysAudioBuffer *GetBuffer() const { return m_buffer; }

        void SetName(const char *name) { m_name = name; }
        std::string GetName() const { return m_name; }

    protected:
        ysAudioBuffer *m_buffer;
        std::string m_name;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_AUDIO_ASSET_H */
