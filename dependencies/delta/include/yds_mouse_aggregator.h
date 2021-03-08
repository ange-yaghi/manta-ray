#ifndef YDS_MOUSE_AGGREGATOR_H
#define YDS_MOUSE_AGGREGATOR_H

#include "yds_mouse.h"

#include "yds_expanding_array.h"

class ysMouseAggregator : public ysMouse {
public:
    ysMouseAggregator();
    virtual ~ysMouseAggregator();

    void RegisterMouse(ysMouse *mouse);
    void DeleteMouse(ysMouse *mouse);
    int GetMouseCount() const { return m_mice.GetNumObjects(); }
    ysMouse *GetMouse(int index) { return m_mice[index]; }

    virtual int GetOsPositionX() const;
    virtual int GetOsPositionY() const;

    virtual int GetX() const;
    virtual int GetY() const;
    virtual int GetWheel() const;
    virtual bool IsDown(Button button) const;

    virtual bool ProcessMouseButton(Button button, ButtonState state);

protected:
    ysExpandingArray<ysMouse *, 4> m_mice;
};

#endif /* YDS_MOUSE_AGGREGATOR_H */
