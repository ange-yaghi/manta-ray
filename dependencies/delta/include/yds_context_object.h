#ifndef YDS_CONTEXT_OBJECT_H
#define YDS_CONTEXT_OBJECT_H

#include "yds_base.h"

#include <string>

class ysContextObject : public ysObject {
public:
    enum class DeviceAPI {
        Unknown,
        DirectX10,
        DirectX11,
        OpenGL4_0,
        Vulkan
    };

public:
    ysContextObject();
    ysContextObject(const char *typeID, DeviceAPI API);
    virtual ~ysContextObject();

    DeviceAPI GetAPI() const { return m_api; }

    bool CheckCompatibility(const ysContextObject *object) const;

    virtual void SetDebugName(const std::string &debugName) { m_debugName = debugName; }
    std::string GetDebugName() const { return m_debugName; }

private:
    DeviceAPI m_api;
    std::string m_debugName;
};

#endif /* YDS_CONTEXT_OBJECT_H */
