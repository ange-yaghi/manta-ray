#ifndef YDS_AUDIO_FILE_H
#define YDS_AUDIO_FILE_H

#include "yds_window_system_object.h"

#include "yds_audio_parameters.h"

class ysAudioBuffer;

class ysAudioFile : public ysWindowSystemObject {
public:
    enum class AudioFormat {
        Wave,
        Undefined
    };

    enum class Error {
        None,

        CouldNotOpenFile,
        InvalidFileFormat,

        FileAlreadyOpen,
        NoFileOpen,
        OutOfMemory,

        ReadOutOfRange,
        FileReadError,

        CouldNotLockBuffer,

        InvalidParam,
    };

public:
    ysAudioFile();
    ysAudioFile(Platform platform, AudioFormat format);
    ~ysAudioFile();

    virtual Error OpenFile(const char *fname);
    Error FillBuffer(SampleOffset offset);
    virtual Error CloseFile();

    SampleOffset GetSampleCount() const { return m_sampleCount; }

    Error InitializeInternalBuffer(SampleOffset samples, bool saveData = false);
    void DestroyInternalBuffer();

    Error AttachExternalBuffer(ysAudioBuffer *buffer);

    SampleOffset GetCurrentReadingOffset() const { return m_currentReadingOffset; }
    const ysAudioParameters *GetAudioParameters() const { return &m_audioParameters; }

    SampleOffset GetBufferSize() const { return (m_buffer) ? m_maxBufferSamples : 0; }
    const void *GetBuffer() const { return m_buffer; }

protected:
    // General Read Functions
    virtual Error GenericRead(SampleOffset offset, SampleOffset size, void *target);

    ysAudioParameters m_audioParameters;

    // Audio Data Parameters
    SampleOffset m_sampleCount;

    // Software Buffer
    ysAudioBuffer *m_externalBuffer;

    // Internal Buffer
    SampleOffset m_maxBufferSamples;
    SampleOffset m_bufferDataSamples;
    char *m_buffer;

    // Streaming
    SampleOffset m_currentReadingOffset;

protected:
    AudioFormat m_format;
    bool m_fileOpen;
};

#endif /* YDS_AUDIO_FILE_H */
