#ifndef YDS_KEYBOARD_H
#define YDS_KEYBOARD_H

#include "yds_window_system_object.h"

class ysKey {
public:
    ysKey();
    ~ysKey();

    enum class Code {
        Back,
        Tab,

        Clear,

        Return,
        Shift,
        Control,

        Menu,
        Pause,
        CapsLock,

        Escape,
        Convert,
        NonConvert,
        Accept,
        ModeChange,

        Space,
        PageUp,
        PageDown,
        End,
        Home,

        Left,
        Up,
        Right,
        Down,

        Select,
        Print,
        PrintScreen,
        Insert,
        Delete,
        Help,

        N0,
        N1,
        N2,
        N3,
        N4,
        N5,
        N6,
        N7,
        N8,
        N9,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        LeftWin,
        RightWin,
        Apps,

        Sleep,

        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,

        Multiply,
        Add,
        Separator,
        Subtract,
        Decimal,
        Divide,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        NumLock,
        ScrollLock,

        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftMenu,
        RightMenu,

        BrowserBack,
        BrowserForward,
        BrowserRefresh,
        BrowserStop,

        BrowserSearch,
        BrowserFavorites,
        BrowserHome,

        VolumeMute,
        VolumeDown,
        VolumeUp,
        MediaNextTrack,
        MediaPrevTrack,
        MediaStop,
        MediaPlay,
        LaunchMail,
        LaunchMediaSelect,

        LaunchApp1,
        LaunchApp2,

        OEM_Plus,
        OEM_Comma,
        OEM_Minus,
        OEM_Period,
        OEM_Clear,

        OEM_1,
        OEM_2,
        OEM_3,
        OEM_4,
        OEM_5,
        OEM_6,
        OEM_7,
        OEM_8,

        Play,
        Zoom,

        Function,

        Undefined,
        Count = Undefined
    };

    enum class State {
        Undefined,

        Up,
        Down,

        UpTransition,
        DownTransition
    };

    enum class Variation {
        Undefined,

        Right,
        Left
    };

    State m_state;
    Variation m_configuration;

    void Reset();

    bool IsDown() const { return m_state == ysKey::State::Down || m_state == ysKey::State::DownTransition; }
};

class ysKeyboard {
    static const char CharMap[];
    static const char CharMapUpper[];

public:
    ysKeyboard();
    virtual ~ysKeyboard();

    void RegisterKeyMap(const ysKey::Code *map) { m_keyMap = map; }
    inline ysKey::Code GetKeyMap(int sysCode) { return m_keyMap[sysCode]; }

    virtual bool ProcessKeyTransition(ysKey::Code key, ysKey::State state = ysKey::State::DownTransition);

    const ysKey *GetKey(ysKey::Code key) { return &m_keys[(int)key];  }

    virtual void SetKeyState(ysKey::Code key, ysKey::State state, ysKey::Variation conf);
    virtual bool IsKeyDown(ysKey::Code key) { return m_keys[(int)key].IsDown(); }

    const char *GetInputBuffer() const { return m_inputBuffer; }
    void ClearInputBuffer();

protected:
    const ysKey::Code *m_keyMap;
    ysKey *m_keys;

    char m_inputBuffer[256];
    int m_inputBufferOffset;
};

#endif /* YDS_KEYBOARD_H */
