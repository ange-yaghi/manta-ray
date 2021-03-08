#ifndef YDS_MOUSE_H
#define YDS_MOUSE_H

class ysMouse {
public:
    enum class Button {
        Left,
        Right,
        Middle,

        Aux_1,
        Aux_2,
        Aux_3,
        Aux_4,
        Aux_5,

        Count
    };

    enum class ButtonState {
        Up,
        Down,

        UpTransition,
        DownTransition
    };

public:
    ysMouse();
    virtual ~ysMouse();

    void Reset();
    void UpdatePosition(int x, int y, bool delta = true);
    void UpdateWheel(int dwheel);
    void UpdateButton(Button button, ButtonState state);
    void SetOsPosition(int x, int y);

    virtual int GetOsPositionX() const { return m_osPosition_x; }
    virtual int GetOsPositionY() const { return m_osPosition_y; }

    virtual int GetX() const { return m_x; }
    virtual int GetY() const { return m_y; }
    virtual int GetWheel() const { return m_wheel; }
    virtual bool IsDown(Button button) const;

    virtual bool ProcessMouseButton(Button button, ButtonState state);

protected:
    int m_x;
    int m_y;
    int m_wheel;

    int m_osPosition_x;
    int m_osPosition_y;

protected:
    ButtonState m_buttonStates[(int)Button::Count];
};

#endif /* YDS_MOUSE_H */
