#ifndef DELTA_BASIC_MASS_SPRING_SYSTEM_H
#define DELTA_BASIC_MASS_SPRING_SYSTEM_H

#include "delta_core.h"

namespace dphysics {

    class MSSParticle;
    class DeltaEngine;

    class MSSSpring : public ysObject {
    public:
        MSSSpring();
        ~MSSSpring();

        virtual float GetLength(float timeDelta = 0.0f) const { return m_length; }
        void SetLength(float length) { m_length = length; }

        float GetConstant() const { return m_constant; }
        void SetConstant(float constant) { m_constant = constant; }

        void SetParticle0(MSSParticle *particle);
        MSSParticle *GetParticle0() { return m_particle0; }

        void SetParticle1(MSSParticle *particle);
        MSSParticle *GetParticle1() { return m_particle1; }

        virtual void Update(float timeDelta) {}

        void SetInvertedForce(bool invertedForce) { m_invertedForce = invertedForce; }
        bool IsInvertedForce() const { return m_invertedForce; }

    protected:
        float m_length;
        float m_constant;

        bool m_invertedForce;

        MSSParticle *m_particle0;
        MSSParticle *m_particle1;
    };

    class MassSpringSystem : public ysObject {
    public:
        enum RK4_PASS {
            RK4_FIRST_PASS,
            RK4_SECOND_PASS,
            RK4_THIRD_PASS,
            RK4_FOURTH_PASS
        };

    public:
        MassSpringSystem();
        ~MassSpringSystem();

        void SetStep(float step);
        float GetStep() const { return m_step; }
        void Update();
        void DrawDebug() {}

        MSSParticle *NewParticle();
        void RemoveParticle(MSSParticle *particle);

        MSSSpring *NewSpring();
        void RemoveSpring(MSSSpring *spring);

        template <typename SpringType>
        SpringType *NewGenericSpring() {
            return m_springs.NewGeneric<SpringType>(16);
        }

        void SetEngine(DeltaEngine *engine) { m_engine = engine; }

    protected:
        DeltaEngine *m_engine;

        float m_step;
        float m_halfStep;
        float m_sixthStep;

        void DetectCollisions();

        ysDynamicArray<MSSParticle, 4> m_particles;
        ysDynamicArray<MSSSpring, 4> m_springs;
    };

    class MSSParticle : public ysObject {
        friend MassSpringSystem;

    public:
        MSSParticle();
        ~MSSParticle();

        ysVector CalculateAcceleration(MassSpringSystem::RK4_PASS pass, float timeDelta);

        void RegisterSpring(MSSSpring *spring);
        void UnregisterSpring(MSSSpring *spring);
        void DeleteAllConnections();

        void SetExternalAcceleration(const ysVector &externalAcceleration) { m_externalAcceleration = externalAcceleration; }
        ysVector GetExternalAcceleration() const { return m_externalAcceleration; }

        void SetPosition(ysVector position) { m_position = position; }
        ysVector GetPosition() const { return m_position; }
        ysVector GetPosition(MassSpringSystem::RK4_PASS pass) const;

        void SetVelocity(ysVector velocity) { m_velocity = velocity; }
        ysVector GetVelocity() const { return m_velocity; }
        ysVector GetVelocity(MassSpringSystem::RK4_PASS pass) const;

        void SetInverseMass(float inverseMass) { m_inverseMass = inverseMass; }
        float GetInverseMass() const { return m_inverseMass; }

        ysVector ExternalAcceleration(MassSpringSystem::RK4_PASS pass);

        void SetDrag(float drag) { m_drag = drag; }
        float GetDrag() const { return m_drag; }

        void SetCollisionEnable(bool enable) { m_enableCollisions = enable; }
        bool GetCollisionEnable() const { return m_enableCollisions; }

        void SetRadius(float radius) { m_radius = radius; }
        float GetRadius() const { return m_radius; }

        void ClearCollisions();
        void AddCollision(MSSParticle *particle);

        int GetCollisionCount() const { return m_collidingParticles.GetNumObjects(); }
        MSSParticle *GetCollision(int i) { return m_collidingParticles[i]; }

        void SetOwner(void *owner) { m_owner = owner; }
        void *GetOwner() { return m_owner; }

        bool IsConnected(MSSParticle *particle);

        ysVector SampleAverageVelocity();
        int GetAverageVelocitySamples() const { return m_averageVelocitySamples; }

    protected:
        float m_inverseMass;
        float m_drag;

        bool m_enableCollisions;
        float m_radius;

        // Store a running average of the velocity
        ysVector m_averageVelocity;
        int m_averageVelocitySamples;

        ysVector m_position;
        ysVector m_velocity;
        ysVector m_externalAcceleration;

        ysVector m_DPTemp1;
        ysVector m_DVTemp1;
        ysVector m_DPTemp2;
        ysVector m_DVTemp2;
        ysVector m_DPTemp3;
        ysVector m_DVTemp3;
        ysVector m_DPTemp4;
        ysVector m_DVTemp4;
        ysVector m_PTemp;
        ysVector m_VTemp;

        ysExpandingArray<MSSSpring *, 4> m_adjacentSprings;
        ysExpandingArray<MSSParticle *, 4> m_collidingParticles;

    protected:
        void *m_owner;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_MASS_SPRING_SYSTEM_H */
