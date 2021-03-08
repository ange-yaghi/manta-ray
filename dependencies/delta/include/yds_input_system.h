#ifndef YDS_INPUT_SYSTEM_H
#define YDS_INPUT_SYSTEM_H

#include "yds_window_system_object.h"

#include "yds_input_device.h"
#include "yds_keyboard_aggregator.h"
#include "yds_mouse_aggregator.h"

class ysWindowSystem;

class ysInputSystem : public ysWindowSystemObject {
protected:
    ysInputSystem();
    ysInputSystem(Platform platform);
    virtual ~ysInputSystem();

public:
    static ysError CreateInputSystem(ysInputSystem **newInputSystem, Platform platform);
    static ysError DestroyInputSystem(ysInputSystem *&inputSystem);

    /* Public Functions */

    ysError Initialize();

    // Assign a window system to this input system.
    ysError AssignWindowSystem(ysWindowSystem *system);

    // Retrieve the current device count
    int GetDeviceCount() const { return m_inputDeviceArray.GetNumObjects(); }

    // Retrieve a free ID to assign to a new device.
    int GetNextDeviceID(ysInputDevice::InputDeviceType type);

    // Retrieve an input device based on its ID and type.
    ysInputDevice *GetInputDevice(int id, ysInputDevice::InputDeviceType type);

    // Check the current state of a device. If a device is found to no
    // longer exist, it is either deleted or disconnected depending on 
    // whether it has any dependencies.
    virtual ysError CheckDeviceStatus(ysInputDevice *device);

    // Check the states of all devices. See CheckDeviceStatus()
    virtual ysError CheckAllDevices();

    ysKeyboardAggregator *GetKeyboardAggregator() { return &m_keyboardAggregator; }
    ysMouseAggregator *GetMouseAggregator() { return &m_mouseAggregator; }

protected:
    // Create and register input devices. This function must be called
    // before any input can be processed.
    virtual ysError CreateDevices() = 0;

    // Create a new generic device. This device will not be attached to a 
    // physical device until one is sensed at which point it will be connected.
    virtual ysInputDevice *CreateDevice(ysInputDevice::InputDeviceType type, int id) = 0;

    void RegisterDevice(ysInputDevice *device);
    void UnregisterDevice(ysInputDevice *device);

    // Disconnect a device. If the device has no dependencies (ie not in use),
    // it will also be deleted.
    virtual void DisconnectDevice(ysInputDevice *device);

    // Find a generic slot. Retrieves the first device that is currently not
    // attached to a physical device.
    ysInputDevice *FindGenericSlot(ysInputDevice::InputDeviceType type);

protected:
    ysKeyboardAggregator m_keyboardAggregator;
    ysMouseAggregator m_mouseAggregator;

    // Input device array
    ysDynamicArray<ysInputDevice, 4> m_inputDeviceArray;

    // Currently assigned window system
    ysWindowSystem *m_windowSystem;
};

#endif /* YDS_INPUT_SYSTEM_H */
