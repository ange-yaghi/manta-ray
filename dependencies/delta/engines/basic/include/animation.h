#ifndef DELTA_BASIC_ANIMATION_H
#define DELTA_BASIC_ANIMATION_H

#include "delta_core.h"

namespace dbasic {

    class DeltaEngine;
    class AssetLibrary;

    class Animation : public ysObject {
        friend DeltaEngine;
        friend AssetLibrary;

    public:
        enum ANIMATION_MODE {
            PAUSED,
            PLAY,
            REVERSE,
            LOOP
        };

    public:
        Animation();
        ~Animation();

        ysTexture *GetCurrentFrame();

        void SetFrame(int frame);
        void SetRate(float rate);
        void Update(float timePassed);

        void SetMode(ANIMATION_MODE mode);

    protected:
        int m_nFrames;

        int m_frame;
        float m_invRate;
        float m_clock;
        ANIMATION_MODE m_mode;

        ysTexture **m_textures;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ANIMATION_H */
