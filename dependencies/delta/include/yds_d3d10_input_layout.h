#ifndef YDS_D3D10_INPUT_LAYOUT_H
#define YDS_D3D10_INPUT_LAYOUT_H

#include "yds_input_layout.h"

#include <D3D10.h>

class ysD3D10InputLayout : public ysInputLayout
{

    friend class ysD3D10Device;

public:

    ysD3D10InputLayout();
    virtual ~ysD3D10InputLayout();

protected:

    ID3D10InputLayout *m_layout;

};

#endif