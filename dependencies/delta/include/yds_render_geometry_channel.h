#ifndef YDS_RENDER_GEOMETRY_CHANNEL_H
#define YDS_RENDER_GEOMETRY_CHANNEL_H

#include "yds_base.h"

class ysRenderGeometryChannel : public ysObject {
    friend class ysRenderGeometryFormat;

public:
    static const int MAX_NAME_LENGTH = 128;

    enum class ChannelFormat {
        R32G32_FLOAT,
        R32G32B32_FLOAT,
        R32G32B32A32_FLOAT,
        R32G32B32A32_UINT,
        R32G32B32_UINT,
        Undefined
    };

public:
    ysRenderGeometryChannel();
    ~ysRenderGeometryChannel();

    const char *    GetName()    const { return m_name; }
    int                GetOffset() const { return m_offset; }
    int                GetSize()    const { return GetFormatSize(m_format); }
    int                GetLength() const { return GetFormatLength(m_format); }
    ChannelFormat    GetFormat() const { return m_format; }

    static int GetFormatSize(ChannelFormat format) {
        switch (format) {
        case ChannelFormat::R32G32_FLOAT:
            return 2 * sizeof(float);
        case ChannelFormat::R32G32B32_FLOAT:
            return 3 * sizeof(float);
        case ChannelFormat::R32G32B32A32_FLOAT:
            return 4 * sizeof(float);
        case ChannelFormat::R32G32B32A32_UINT:
            return 4 * sizeof(unsigned int);
        case ChannelFormat::R32G32B32_UINT:
            return 3 * sizeof(unsigned int);
        case ChannelFormat::Undefined:
        default:
            return 0;
        }
    }

    static int GetFormatLength(ChannelFormat format) {
        switch(format) {
        case ChannelFormat::R32G32_FLOAT:
            return 2;
        case ChannelFormat::R32G32B32_FLOAT:
            return 3;
        case ChannelFormat::R32G32B32A32_FLOAT:
            return 4;
        case ChannelFormat::R32G32B32A32_UINT:
            return 4;
        case ChannelFormat::R32G32B32_UINT:
            return 3;
        case ChannelFormat::Undefined:
        default:
            return 0;
        }
    }

protected:
    char            m_name[MAX_NAME_LENGTH];
    int                m_offset;
    ChannelFormat    m_format;
};

#endif /* YDS_RENDER_GEOMETRY_CHANNEL_H */
