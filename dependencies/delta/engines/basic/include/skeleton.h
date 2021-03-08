#ifndef DELTA_BASIC_SKELETON_H
#define DELTA_BASIC_SKELETON_H

#include "delta_core.h"

#include "bone.h"

namespace dbasic {

    class Skeleton : public ysObject {
    public:
        Skeleton();
        ~Skeleton();

        Bone *NewBone();
        Bone *GetBone(int index);
        Bone *GetBone(const char *boneName);

        int GetBoneCount() const { return m_bones.GetNumObjects(); }
        Bone *FindBone(const char *boneName);

        void Update();

        void SetRootBone(Bone *bone) { m_rootBone = bone; }
        Bone *GetRootBone() { return m_rootBone; }

    protected:
        Bone *m_rootBone;

        ysDynamicArray<Bone, 4> m_bones;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SKELETON_H */
