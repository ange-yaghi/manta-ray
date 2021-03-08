#ifndef YDS_D3D11_INPUT_LAYOUT_H
#define YDS_D3D11_INPUT_LAYOUT_H

#include "yds_input_layout.h"

#include <D3D11.h>

class ysD3D11InputLayout : public ysInputLayout {
    friend class ysD3D11Device;

public:
    ysD3D11InputLayout();
    virtual ~ysD3D11InputLayout();

    virtual void SetDebugName(const std::string &debugName);

protected:
    ID3D11InputLayout *m_layout;

};

#endif /* YDS_D3D11_INPUT_LAYOUT_H */
