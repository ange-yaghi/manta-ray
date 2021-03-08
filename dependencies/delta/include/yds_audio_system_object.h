#ifndef YDS_AUDIO_SYSTEM_OBJECT_H
#define YDS_AUDIO_SYSTEM_OBJECT_H

#include "yds_base.h"

class ysAudioSystemObject : public ysObject {
public:
    enum class API {
        Undefined,
        DirectSound8,
    };

public:
    ysAudioSystemObject();
    ysAudioSystemObject(const char *typeID, API API);
    virtual ~ysAudioSystemObject();

    API GetAPI() const { return m_api; }

    bool CheckCompatibility(ysAudioSystemObject *object) const { return (object) ? object->m_api == m_api : true; }

private:
    API m_api;
};

#endif /* YDS_AUDIO_SYSTEM_OBJECT_H */
