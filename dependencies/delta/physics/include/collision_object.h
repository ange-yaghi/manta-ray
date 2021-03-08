#ifndef DELTA_BASIC_COLLISION_OBJECT_H
#define DELTA_BASIC_COLLISION_OBJECT_H

#include "delta_core.h"

#include "collision_primitives.h"

namespace dphysics {

    class RigidBody;
    class CollisionObject : public ysObject {
    public:
        enum class Mode {
            Sensor = 0x1,

            Fine = 0x2,
            Coarse = 0x4,

            Undefined,
        };

        enum class Type {
            Circle,
            Box,
            Ray,

            Undefined
        };

    public:
        CollisionObject();
        CollisionObject(Type type);
        virtual ~CollisionObject();

        inline Type GetType() const { return m_type; }

        BoxPrimitive *GetAsBox() const { return static_cast<BoxPrimitive *>(m_primitiveHandle); }
        CirclePrimitive *GetAsCircle() const { return static_cast<CirclePrimitive *>(m_primitiveHandle); }
        RayPrimitive *GetAsRay() const { return static_cast<RayPrimitive *>(m_primitiveHandle); }

        Mode GetMode() const { return m_mode; }
        void SetMode(Mode mode) { m_mode = mode; }

        void SetParent(RigidBody *parent) { m_parent = parent; }
        RigidBody *GetParent() const { return m_parent; }

        void SetRelativePosition(const ysVector &pos) { m_relativePosition = pos; }
        void SetRelativeOrientation(const ysQuaternion &orientation) { m_relativeOrientation = orientation; }

        ysVector GetRelativePosition() const { return m_relativePosition; }
        ysQuaternion GetRelativeOrientation() const { return m_relativeOrientation; }

        void ConfigurePrimitive();

        void SetCollidesWith(int layer, bool collides);
        bool CollidesWith(int layer) const;

        void SetLayer(int layer);
        int GetLayer() const { return m_layer; }

        bool CheckCollisionMask(const CollisionObject *object) const;

        void SetEventMessage(unsigned int msg) { m_msg = msg; }
        unsigned int GetEventMessage() const { return m_msg; }

        void GetBounds(ysVector &minPoint, ysVector &maxPoint) const;
        void WriteInfo(std::fstream &target);

    protected:
        void ConfigureBox();
        void ConfigureCircle();
        void ConfigureRay();

        void *m_primitiveHandle;

        ysVector m_relativePosition;
        ysQuaternion m_relativeOrientation;

        unsigned int m_layer;
        unsigned int m_collisionLayerMask;

    private:
        Type m_type;
        Mode m_mode;

        unsigned int m_msg;

        RigidBody *m_parent;
    };

    template <typename T_Type, CollisionObject::Type typeID>
    class CollisionObjectSpecialized : public CollisionObject {
    public:
        CollisionObjectSpecialized() : CollisionObject(typeID) {
            m_primitiveHandle = (void *)&m_collisionPrimitive;
        }

        virtual ~CollisionObjectSpecialized() {
            /* void */
        }

    protected:
        T_Type m_collisionPrimitive;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_COLLISION_OBJECT_H */
