#ifndef DELTA_BASIC_BONE_H
#define DELTA_BASIC_BONE_H

#include "delta_core.h"

#include "../../../physics/include/rigid_body.h"

namespace dbasic {

    class Skeleton;

    class Bone : public ysObject {
    public:
        Bone();
        ~Bone();

        ysTransform Transform;

        void SetParent(Bone *parent);
        Bone *GetParent() { return m_parent; }

        const char *GetName() { return m_name; }
        void SetName(const char *name) { strcpy_s(m_name, 64, name); }

        Skeleton *GetSkeleton() { return m_skeleton; }
        void SetSkeleton(Skeleton *skeleton) { m_skeleton = skeleton; }

        void SetReferenceTransform(const ysMatrix &m);

        void LocalSpace(ysMatrix &m);

        ysMatrix GetSkinMatrix();

        void SetAssetID(int assetID) { m_assetID = assetID; }
        int GetAssetID() const { return m_assetID; }

    protected:
        char m_name[64];
        int m_assetID;

        ysMatrix m_referenceTransform;
        ysMatrix m_inverseReferenceTransform;

        Bone *m_parent;

        Skeleton *m_skeleton;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_BONE_H */
