#ifndef DELTA_BASIC_RENDER_SKELETON_H
#define DELTA_BASIC_RENDER_SKELETON_H

#include "delta_core.h"

#include "render_node.h"

namespace dbasic {

    class RenderSkeleton : public ysObject {
    public:
        RenderSkeleton();
        ~RenderSkeleton();

        RenderNode *GetRoot();
        RenderNode *NewNode();
        RenderNode *GetNode(int index);
        RenderNode *GetNode(const char *nodeName);

        void BindAction(ysAnimationAction *action, ysAnimationActionBinding *binding);

        int GetNodeCount() const { return m_renderNodes.GetNumObjects(); }
        RenderNode *FindNode(const char *nodeName);

        void Update();
        void UpdateAnimation(float dt);

        ysAnimationMixer AnimationMixer;

    protected:
        // Container for all nodes
        ysDynamicArray<RenderNode, 4> m_renderNodes;
    };
    
} /* namespace dbasic */

#endif /* DELTA_BASIC_RENDER_SKELETON_H */
