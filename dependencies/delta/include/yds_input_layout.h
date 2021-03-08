#ifndef YDS_INPUT_LAYOUT_H
#define YDS_INPUT_LAYOUT_H

#include "yds_context_object.h"

#include "yds_render_geometry_format.h"
#include "yds_shader.h"

class ysInputLayout : public ysContextObject {
    friend class ysDevice;

public:
    ysInputLayout();
    ysInputLayout(DeviceAPI API);
    virtual ~ysInputLayout();

protected:
    // The format used to create this input layout
    ysRenderGeometryFormat *m_format;

    // The shader used to create this input layout
    ysShader *m_shader;
};

#endif /* YDS_INPUT_LAYOUT_H */
