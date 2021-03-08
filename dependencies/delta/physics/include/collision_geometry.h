#ifndef DELTA_BASIC_COLLISION_GEOMETRY_H
#define DELTA_BASIC_COLLISION_GEOMETRY_H

#include "delta_core.h"

#include "collision_object.h"

namespace dphysics {

    class RigidBody;
    class CollisionGeometry : public ysObject {
    public:
        CollisionGeometry();
        ~CollisionGeometry();

        ysError NewBoxObject(CollisionObject **newObject);
        ysError NewCircleObject(CollisionObject **newObject);
        ysError NewRayObject(CollisionObject **newObject);

        inline int GetNumObjects() const { return m_collisionObjects.GetNumObjects(); }
        CollisionObject *GetCollisionObject(int index) { return m_collisionObjects.Get(index); }
        inline CollisionObject **GetCollisionObjects() { return m_collisionObjects.GetBuffer(); }

        void UpdatePrimitives();

        void SetParent(RigidBody *parent) { m_parent = parent; }

    protected:
        ysDynamicArray<CollisionObject, 4> m_collisionObjects;
        RigidBody *m_parent;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_COLLISION_GEOMETRY_H */
