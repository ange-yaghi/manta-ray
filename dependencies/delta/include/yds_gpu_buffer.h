#ifndef YDS_GPU_BUFFER_H
#define YDS_GPU_BUFFER_H

#include "yds_context_object.h"

class ysDevice;

class ysGPUBuffer : public ysContextObject {
    friend ysDevice;

public:
    enum GPU_BUFFER_TYPE {
        GPU_DATA_BUFFER,
        GPU_INDEX_BUFFER,
        GPU_CONSTANT_BUFFER,
        GPU_UNDEFINED_BUFFER
    };

public:
    ysGPUBuffer();
    ysGPUBuffer(DeviceAPI API);
    virtual ~ysGPUBuffer();

    int GetSize() const { return m_size; }
    const char *GetRAMMirror() const { return m_RAMMirror; }

    GPU_BUFFER_TYPE GetType() const { return m_bufferType; }

protected:
    GPU_BUFFER_TYPE m_bufferType;

    char *m_RAMMirror;
    int m_size;

    int m_currentStride;

    bool m_mirrorToRAM;
};

#endif /* YDS_GPU_BUFFER_H */
