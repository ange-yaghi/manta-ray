#ifndef YDS_OPENGL_INPUT_LAYOUT_H
#define YDS_OPENGL_INPUT_LAYOUT_H

#include "yds_input_layout.h"

#include "yds_dynamic_array.h"

class ysOpenGLDevice;

struct ysOpenGLLayoutChannel : public ysDynamicArrayElement {
    int m_length = 0;
    int m_size = 0;
    int m_offset = 0;
    int m_type = 0;
};

class ysOpenGLInputLayout : public ysInputLayout {
    friend ysOpenGLDevice;

public:
    ysOpenGLInputLayout();
    virtual ~ysOpenGLInputLayout();

protected:
    int m_size;

    ysDynamicArray<ysOpenGLLayoutChannel, 16> m_channels;
};

#endif /* YDS_OPENGL_INPUT_LAYOUT_H */
