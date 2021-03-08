#ifndef DELTA_BASIC_PARTICLE_H
#define DELTA_BASIC_PARTICLE_H

#include "delta_core.h"

namespace dphysics {

    class DeltaEngine;
    class ParticleSystem;

    class Particle : public ysObject {
        friend ParticleSystem;

    public:
        Particle();
        ~Particle();

        void SetParent(ParticleSystem *parent) { m_parent = parent; }

        float GetAge() const { return m_age; }
        float GetLife() const { return m_life; }

        void Update(float timePassed);

    protected:
        ParticleSystem *m_parent;

        ysTexture *m_texture;

        ysVector m_position;
        ysVector m_velocity;

        float m_life;
        float m_age;
        float m_scale;
        float m_damping;
        float m_rotation;
        float m_expansionRate;
        float m_density;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_PARTICLE_H */
