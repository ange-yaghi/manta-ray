#ifndef DELTA_BASIC_PARTICLE_SYSTEM_H
#define DELTA_BASIC_PARTICLE_SYSTEM_H

#include "delta_core.h"

#include "particle.h"

namespace dphysics {

    class DeltaEngine;

    class ParticleSystem : public ysObject {
    public:
        ParticleSystem();
        ~ParticleSystem();

        ysDynamicArray<Particle, 512> m_particles;

        void Update(float timeStep);
        void SetEngine(DeltaEngine *engine) { m_engine = engine; }
        void SetRate(float rate) { m_rate = rate; }
        void SetTexture(ysTexture *texture) { m_texture = texture; }
        void SetPosition(ysVector position) { m_source = position; }
        void SetLayer(int layer) { m_layer = layer; }

        int GetParticleCount() const { return m_particles.GetNumObjects(); }

        ysVector GetPosition() const { return m_source; }

    protected:
        DeltaEngine *m_engine;

        ysVector m_source;
        ysVector m_direction;

        ysTexture *m_texture;

        // Number of particles per second
        float m_rate;

        int m_layer;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_PARTICLE_SYSTEM_H */
