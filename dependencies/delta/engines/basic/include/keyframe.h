#ifndef DELTA_BASIC_KEYFRAME_H
#define DELTA_BASIC_KEYFRAME_H

#include "delta_core.h"

#include "animation_export_data.h"

namespace dbasic {

    class Keyframe : public ysObject {
    public:
        Keyframe();
        ~Keyframe();

        unsigned int GetKeyType() const { return m_keyType; }

        int GetFrame() const { return m_frame; }

        ysVector GetPositionKey() const { return m_positionKey; }
        ysQuaternion GetRotationKey() const { return m_rotationKey; }

        void LoadAssetKeyframe(ObjectKeyframeDataExport::KEY_DATA *data);

    protected:
        unsigned int m_keyType;

        int m_frame;

        ysVector        m_positionKey;
        ysQuaternion    m_rotationKey;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_KEYFRAME_H */
