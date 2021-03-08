#ifndef DELTA_BASIC_COLLISION_PRIMITIVES_H
#define DELTA_BASIC_COLLISION_PRIMITIVES_H

#include "delta_core.h"

namespace dphysics {

    struct BoxPrimitive {
        ysQuaternion Orientation;
        ysVector Position;

        union {
            struct {
                float HalfWidth;
                float HalfHeight;
            };
            float Extents[2];
        };

        void GetBounds(ysVector &minPoint, ysVector &maxPoint) const;
    };

    struct CirclePrimitive {
        ysVector Position;
        float Radius;

        void GetBounds(ysVector &minPoint, ysVector &maxPoint) const;
    };

    struct RayPrimitive {
        ysVector Position;
        ysVector Direction;
        float MaxDistance;

        ysVector RelativeDirection;
    };

    class RigidBody;
    class CollisionObject;
    class RigidBodySystem;

    class Collision : public ysObject {
        friend RigidBodySystem;

    public:
        enum class CollisionType {
            PointFace,
            EdgeEdge,
            Generic,
            Unknown
        };

    public:
        Collision();
        Collision(const Collision &collision);
        ~Collision();

        CollisionType m_collisionType;
        int m_feature1;
        int m_feature2;

        float m_penetration;
        ysVector m_normal;
        ysVector m_position;

        union {
            struct {
                RigidBody *m_body1;
                RigidBody *m_body2;
            };

            RigidBody *m_bodies[2];
        };

        union {
            struct {
                CollisionObject *m_collisionObject1;
                CollisionObject *m_collisionObject2;
            };

            CollisionObject *m_collisionObjects[2];
        };

        bool m_sensor;
        float m_restitution;
        float m_staticFriction;
        float m_dynamicFriction;

        void Initialize();
        void UpdateInternals(float timestep);

        Collision &operator=(Collision &collision);

        bool IsGhost() const;
        bool IsResolvable() const;

        // Get velocity on impact
        ysVector GetContactVelocity() const { return m_initialContactVelocity; }
        ysVector GetContactVelocityWorld() const;

        bool IsSameAs(Collision *other) const;

    protected:
        ysVector m_relativePosition[2];
        ysMatrix m_contactSpace;
        ysVector m_contactVelocity;
        ysVector m_initialContactVelocity;

        float m_desiredDeltaVelocity;

        void CalculateDesiredDeltaVelocity(float timestep);

        ysVector CalculateLocalVelocity(int bodyIndex);

    private:
        void CalculateContactSpace();
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_COLLISION_PRIMITIVES_H */
