#ifndef DELTA_BASIC_CHARACTER_H
#define DELTA_BASIC_CHARACTER_H

#include "delta_core.h"

#include "animation_group.h"
#include "skeleton.h"
#include "asset_manager.h"
#include "render_skeleton.h"

namespace dbasic {

    class Character : public ysObject {
        friend AssetManager;

    public:
        Character(const char *typeName);
        Character();
        ~Character();

        // Update the character
        void Update();

        // Set the skeleton being used by this character
        void SetSkeleton(Skeleton *skeleton);

        // Set the render skeleton used by this character
        void SetRenderSkeleton(RenderSkeleton *renderSkeleton);

        // Set the animation data used by this character
        void SetAnimationData(AnimationExportData *animationData);

        // Construct a controller for a bone
        void ConstructBoneController(const char *boneName, AnimationGroup *group);

        // Construct a controller for a render node
        void ConstructNodeController(const char *nodeName, AnimationGroup *group);

        // Construct the character
        virtual void ConstructCharacter() = 0;

        // Get a reference to the main animation controller
        AnimationGroup *GetAnimationController();

        //protected:

            // Set the asset manager that manages this character
        void SetAssetManager(AssetManager *assetManager);

    protected:
        // Reference to the render skeleton
        RenderSkeleton *m_renderSkeleton;

        // References to the skeleton being controlled
        Skeleton *m_skeleton;

        // Main top level animation controller
        AnimationGroup m_animationController;

        // Animation data like poses, motions, etc.
        AnimationExportData *m_animationData;

        // Reference to top level asset manager
        AssetManager *m_assetManager;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_CHARACTER_H */
