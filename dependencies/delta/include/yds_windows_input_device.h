#ifndef YDS_WINDOWS_INPUT_DEVICE_H
#define YDS_WINDOWS_INPUT_DEVICE_H

#include "yds_input_device.h"

#include <Windows.h>

class ysWindowsInputDevice : public ysInputDevice {
    friend class ysWindowsInputSystem;

public:
    ysWindowsInputDevice();
    ysWindowsInputDevice(InputDeviceType type);
    ~ysWindowsInputDevice();

    void MakeGeneric();

protected:
    HANDLE m_deviceHandle;
    RID_DEVICE_INFO m_info;
    char m_systemName[256];
};

#endif /* YDS_WINDOWS_INPUT_DEVICE_H */
