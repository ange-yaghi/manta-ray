#ifndef DELTA_BASIC_POSE_H
#define DELTA_BASIC_POSE_H

#include "delta_core.h"

#include "keyframe.h"

namespace dbasic {

    class Pose : public ysObject {
    public:
        Pose();
        ~Pose();

        void SetFrame(int frame) { m_frame = frame; }
        int GetFrame() const { return m_frame; }

        const char *GetName() const { return m_name; }

    protected:
        char m_name[64];
        int m_frame;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_POSE_H */
