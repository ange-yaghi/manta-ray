#ifndef YDS_D3D11_UTILITIES_H
#define YDS_D3D11_UTILITIES_H

#include <string>

struct ID3D11DeviceChild;

void D3D11SetDebugName(ID3D11DeviceChild *object, const std::string &name);

#endif /* YDS_D3D11_UTILITIES_H */
