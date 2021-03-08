#ifndef YDS_WINDOW_SYSTEM_OBJECT_H
#define YDS_WINDOW_SYSTEM_OBJECT_H

#include "yds_base.h"

class ysWindowSystemObject : public ysObject {
public:
    enum class Platform {
        Windows,
        Unknown,
    };

public:
    ysWindowSystemObject();
    ysWindowSystemObject(const char *typeID, Platform platform);
    virtual ~ysWindowSystemObject();

    Platform GetPlatform() const { return m_platform; }

    bool CheckCompatibility(ysWindowSystemObject *object) const { return (object) ? object->m_platform == m_platform : true; }

private:
    Platform m_platform;
};

#endif /* YDS_WINDOW_SYSTEM_OBJECT_H */
