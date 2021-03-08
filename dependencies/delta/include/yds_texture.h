#ifndef YDS_TEXTURE_H
#define YDS_TEXTURE_H

#include "yds_context_object.h"

class ysTexture : public ysContextObject {
    friend class ysDevice;

public:
    static const int MAX_FILENAME_LENGTH = 256;

public:
    ysTexture();
    ysTexture(DeviceAPI API);
    virtual ~ysTexture();

    const char *GetFilename() { return m_filename; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

protected:
    int m_width;
    int m_height;

    char m_filename[MAX_FILENAME_LENGTH + 1];
};

#endif /* YDS_TEXTURE_H */
