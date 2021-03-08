#ifndef DELTA_BASIC_ANIMATION_OBJECT_CONTROLLER_H
#define DELTA_BASIC_ANIMATION_OBJECT_CONTROLLER_H

#include "delta_core.h"

#include "keyframe.h"

namespace dbasic {

    class AnimationObjectController : public ysObject {
    public:
        AnimationObjectController();
        ~AnimationObjectController();

        ysTransform *GetTarget() { return m_target; }
        void SetTarget(ysTransform *target) { m_target = target; }

        Keyframe *AppendKeyframe() { return &m_timeline.New(); }

        void SetTimeOffset(float timeOffset) { m_timeOffset = timeOffset; m_frame = (int)(m_timeOffset * m_framerate); }
        void SetFrame(int frame) { m_frame = frame; m_timeOffset = m_frame / m_framerate; }

        void Update();

    protected:
        float m_timeOffset;
        int m_frame;
        float m_framerate;

        ysTransform *m_target;
        ysExpandingArray<Keyframe, 0, 16> m_timeline;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ANIMATION_OBJECT_CONTROLLER_H */
