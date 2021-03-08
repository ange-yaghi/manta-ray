#ifndef DELTA_PHYSICS_FORCE_GENERATOR_H
#define DELTA_PHYSICS_FORCE_GENERATOR_H

#include "delta_core.h"

namespace dphysics {

    class RigidBody;

    class ForceGenerator : public ysDynamicArrayElement {
    public:
        ForceGenerator();
        ~ForceGenerator();

        virtual void GenerateForces(float dt) = 0;

        virtual void Initialize(RigidBody *body) { m_rigidBody = body; }
        RigidBody *GetRigidBody() const { return m_rigidBody; }

    protected:
        RigidBody *m_rigidBody;
    };

} /* namespace dphysics */

#endif /* DELTA_PHYSICS_FORCE_GENERATOR_H */
