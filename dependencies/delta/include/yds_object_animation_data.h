#ifndef YDS_OBJECT_ANIMATION_DATA_H
#define YDS_OBJECT_ANIMATION_DATA_H

#include "yds_math.h"
#include "yds_expanding_array.h"

class ysObjectAnimationData {
public:
    enum ANIMATION_KEY {
        ANIMATION_KEY_POSITION,
        ANIMATION_KEY_ROTATION,

        NUM_KEY_TYPES,
        ANIMATION_KEY_UNDEFINED = NUM_KEY_TYPES
    };

    struct PositionKey {
        int Frame;
        ysVector3 Position;
    };

    struct PositionKeySeries {
        ANIMATION_KEY Type;
        int NumKeys;

        ysExpandingArray<PositionKey> Keys;
    };

    struct RotationKey {
        int Frame;
        ysVector4 RotationQuaternion;
    };

    struct RotationKeySeries {
        ANIMATION_KEY Type;
        int NumKeys;

        ysExpandingArray<RotationKey> Keys;
    };

public:
    ysObjectAnimationData();
    ~ysObjectAnimationData();

    void Clear();

    // Header
    char m_objectName[64];

    PositionKeySeries m_positionKeys;
    RotationKeySeries m_rotationKeys;
};

#endif /* YDS_OBJECT_ANIMATION_DATA_H */
