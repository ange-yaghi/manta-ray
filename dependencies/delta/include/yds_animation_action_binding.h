#ifndef YDS_ANIMATION_ACTION_BINDING_H
#define YDS_ANIMATION_ACTION_BINDING_H

#include <vector>
#include <string>

struct TransformTarget;
class ysAnimationAction;
class ysAnimationTarget;

class ysAnimationActionBinding {
public:
    ysAnimationActionBinding();
    ~ysAnimationActionBinding();

    void Sample(float s, float amplitude);
    void SampleRest(float amplitude);
    void AddTarget(const std::string &name, TransformTarget *locationTarget, TransformTarget *rotationTarget);

    void SetAction(ysAnimationAction *action) { m_action = action; }
    ysAnimationAction *GetAction() const { return m_action; }

protected:
    std::vector<ysAnimationTarget *> m_targets;
    ysAnimationAction *m_action;
};

#endif /* YDS_ANIMATION_ACTION_BINDING_H */
