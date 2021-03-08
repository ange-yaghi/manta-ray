#ifndef DELTA_BASIC_RIGID_BODY_LINK_H
#define DELTA_BASIC_RIGID_BODY_LINK_H

#include "delta_core.h"

namespace dphysics {

    class DeltaEngine;
    class Collision;
    class RigidBody;

    class RigidBodyLink : public ysObject {
    public:
        RigidBodyLink();
        ~RigidBodyLink();

        void SetRigidBodies(RigidBody *body1, RigidBody *body2) { m_body1 = body1; m_body2 = body2; }

        virtual int GenerateCollisions(Collision *collisionArray) { return 0; }
        virtual void DrawDebug(DeltaEngine *engine, int layer) { /* void */ }

    protected:
        union {
            struct {
                RigidBody *m_body1;
                RigidBody *m_body2;
            };

            RigidBody *m_bodies[2];
        };
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_RIGID_BODY_LINK_H */
