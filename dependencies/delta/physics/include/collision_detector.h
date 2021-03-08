#ifndef DELTA_BASIC_COLLISION_DETECTOR_H
#define DELTA_BASIC_COLLISION_DETECTOR_H

#include "collision_primitives.h"
#include "rigid_body.h"

namespace dphysics {

    class CollisionDetector {
    public:
        CollisionDetector();
        ~CollisionDetector();

        int BoxBoxCollision(Collision *collisions, RigidBody *body1, RigidBody *body2, BoxPrimitive *box1, BoxPrimitive *box2);
        int CircleBoxCollision(Collision *collisions, RigidBody *body1, RigidBody *body2, CirclePrimitive *circle, BoxPrimitive *box);
        int CircleCircleCollision(Collision *collisions, RigidBody *body1, RigidBody *body2, CirclePrimitive *circle1, CirclePrimitive *circle2);
        int RayBoxCollision(Collision *collisions, RigidBody *body1, RigidBody *body2, RayPrimitive *ray, BoxPrimitive *box);
        int RayCircleCollision(Collision *collisions, RigidBody *body1, RigidBody *body2, RayPrimitive *ray, CirclePrimitive *circle);

        bool CircleCircleIntersect(RigidBody *body1, RigidBody *body2, CirclePrimitive *circle1, CirclePrimitive *circle2);

        bool _BoxBoxColliding(BoxPrimitive *body1, BoxPrimitive *body2);
        int BoxBoxVertexPenetration(Collision *collisions, BoxPrimitive *body1, BoxPrimitive *body2);
    };

} /* namesapce dbasic */

#endif /* DELTA_BASIC_COLLISION_DETECTOR_H */
