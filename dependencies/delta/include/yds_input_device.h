#ifndef YDS_INPUT_DEVICE_H
#define YDS_INPUT_DEVICE_H

#include "yds_window_system_object.h"

#include "yds_keyboard.h"
#include "yds_mouse.h"

class ysInputSystem;

class ysInputDevice : public ysWindowSystemObject {
    friend ysInputSystem;

public:
    static const int MAX_NAME_LENGTH = 256;

    enum class InputDeviceType {
        KEYBOARD,
        MOUSE,
        CUSTOM,
        UNKNOWN
    };

public:
    ysInputDevice();
    ysInputDevice(Platform platform, InputDeviceType type);
    ~ysInputDevice();

    InputDeviceType GetType() const { return m_type; }

    void SetName(const char *name);
    const char *GetName() const { return m_name; }

    void SetType(InputDeviceType type);

    int GetDeviceID() const { return m_deviceID; }
    void SetDeviceID(int deviceID) { m_deviceID = deviceID; }

    void AttachDependency() { m_dependencyCount++; }
    void DetachDependency() { m_dependencyCount--; }
    int GetDependencyCount() const { return m_dependencyCount; }

    ysKeyboard *GetAsKeyboard();
    ysMouse *GetAsMouse();

    bool IsConnected() const { return m_connected; }
    void SetConnected(bool connected) { m_connected = connected; }

    bool IsGeneric() const { return m_generic; }
    void SetGeneric(bool newGeneric) { m_generic = newGeneric; }

private:
    void Destroy();

    InputDeviceType m_type;
    char m_name[MAX_NAME_LENGTH];

    int m_deviceID;
    int m_dependencyCount;

    // Specific
    union {
        ysKeyboard *m_keyboard;
        ysMouse *m_mouse;

        void *m_genericData;
    };

    bool m_connected;
    bool m_generic;
};

#endif /* YDS_INPUT_DEVICE_H */
