#ifndef YDS_AUDIO_PARAMETERS_H
#define YDS_AUDIO_PARAMETERS_H

typedef unsigned int SampleOffset;

class ysAudioParameters {
public:
    ysAudioParameters();
    ~ysAudioParameters();

    unsigned int GetSizeFromSamples(SampleOffset samples) const { return (unsigned int )((m_bitsPerSample / 8) * m_channelCount * samples); }
    SampleOffset GetSamplesFromSize(unsigned int size) const { return (SampleOffset)(size / ((m_bitsPerSample / 8) * m_channelCount)); }

    int m_channelCount;
    int m_bitsPerSample;
    int m_sampleRate;
};

#endif /* YDS_AUDIO_PARAMETERS_H */
