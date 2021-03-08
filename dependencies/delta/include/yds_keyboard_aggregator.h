#ifndef YDS_KEYBOARD_AGGREGATOR_H
#define YDS_KEYBOARD_AGGREGATOR_H

#include "yds_keyboard.h"

#include "yds_expanding_array.h"

class ysKeyboardAggregator : public ysKeyboard {
public:
    ysKeyboardAggregator();
    virtual ~ysKeyboardAggregator();

    void RegisterKeyboard(ysKeyboard *keyboard);
    void DeleteKeyboard(ysKeyboard *keyboard);
    int GetKeyboardCount() const { return m_keyboards.GetNumObjects(); }
    ysKeyboard *GetKeyboard(int index) { return m_keyboards[index]; }

    virtual bool IsKeyDown(ysKey::Code key);
    virtual bool ProcessKeyTransition(ysKey::Code key, ysKey::State state = ysKey::State::DownTransition);

    virtual void SetKeyState(ysKey::Code key, ysKey::State state, ysKey::Variation conf);
    virtual const ysKey *GetKey(ysKey::Code key) const;

protected:
    ysExpandingArray<ysKeyboard *, 4> m_keyboards;
};

#endif /* YDS_KEYBOARD_AGGREGATOR_H */
