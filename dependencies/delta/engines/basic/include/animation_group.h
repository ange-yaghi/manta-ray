#ifndef DELTA_BASIC_ANIMATION_GROUP_H
#define DELTA_BASIC_ANIMATION_GROUP_H

#include "delta_core.h"

#include "animation_object_controller.h"

namespace dbasic {

    class AnimationGroup : public ysObject {
    public:
        AnimationGroup();
        ~AnimationGroup();

        // Update all the animation controllers
        void Update();

        // Add an animation controller
        void AddAnimationController(AnimationObjectController *controller);

        // Add a sub-group
        AnimationGroup *AddAnimationGroup(const char *groupName);

        // Set name
        void SetName(const char *name);

        // Set frame
        void SetFrame(int frame);
        void SetTimeOffset(float timeOffset);

    protected:
        // The animation group name
        char m_name[64];

        // List of animation controllers that are part of this animation domain
        ysExpandingArray<AnimationObjectController *, 16> m_animationControllers;

        // Child animation groups
        ysDynamicArray<AnimationGroup, 16> m_animationGroups;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_ANIMATION_GROUP_H */
