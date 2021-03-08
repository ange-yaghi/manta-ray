#ifndef YDS_DIRECT_SOUND8_AUDIO_BUFFER_H
#define YDS_DIRECT_SOUND8_AUDIO_BUFFER_H

#include "yds_audio_buffer.h"

class ysDS8AudioBuffer : public ysAudioBuffer {
public:
    ysDS8AudioBuffer();
    virtual ~ysDS8AudioBuffer();

    virtual ysError Initialize(SampleOffset samples, const ysAudioParameters &parameters);
    virtual ysError EditBuffer(void *data);
    virtual ysError EditBufferSegment(void *data, SampleOffset samples, SampleOffset offset);
    virtual ysError Destroy();

    void *GetBuffer() const { return m_buffer; }

protected:
    void *m_buffer;
};

#endif /* YDS_DIRECT_SOUND8_AUDIO_BUFFER_H */