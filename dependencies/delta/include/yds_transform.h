#ifndef YDS_TRANSFORM_H
#define YDS_TRANSFORM_H

#include "yds_math.h"

class ysTransform {
public:
    ysTransform();
    ~ysTransform();

    void SetPosition(const ysVector &position);
    void SetOrientation(const ysQuaternion &q);

    ysMatrix GetWorldTransform();

    ysVector WorldToLocalSpace(const ysVector &p);
    ysVector WorldToParentSpace(const ysVector &p);
    ysVector LocalToWorldSpace(const ysVector &p);
    ysVector LocalToParentSpace(const ysVector &p);

    ysVector WorldToLocalDirection(const ysVector &dir);
    ysVector WorldToParentDirection(const ysVector &dir);
    ysVector LocalToWorldDirection(const ysVector &dir);
    ysVector LocalToParentDirection(const ysVector &dir);

    ysVector ParentToLocalSpace(const ysVector &p);
    ysVector ParentToWorldSpace(const ysVector &p);
    ysVector ParentToLocalDirection(const ysVector &p);
    ysVector ParentToWorldDirection(const ysVector &p);

    ysQuaternion WorldToLocalOrientation(const ysQuaternion &q);
    ysQuaternion WorldToParentOrientation(const ysQuaternion &q);

    ysQuaternion GetOrientationParentSpace() const;
    ysQuaternion GetWorldOrientation();

    ysVector GetPositionParentSpace() const;
    ysVector GetWorldPosition();

    void SetParent(ysTransform *parent);
    ysTransform *GetParent() const { return m_parent; }

    bool IsValid();

private:
    ysVector m_position;
    ysQuaternion m_orientation;

    ysTransform *m_parent;
};

#endif /* YDS_TRANSFORM_H */
