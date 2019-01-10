#ifndef BATCHED_MONTE_CARLO_EMITTER_GROUP_H
#define BATCHED_MONTE_CARLO_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <batched_monte_carlo_emitter.h>

namespace manta {

	class BatchedMonteCarloEmitterGroup : public RayEmitterGroup {
	public:
		BatchedMonteCarloEmitterGroup();
		virtual ~BatchedMonteCarloEmitterGroup();

		virtual void createAllEmitters();

		BatchedMonteCarloEmitter *m_simpleRayEmitter;

		void setTexCoord(const math::Vector &texCoord) { m_texCoord = texCoord; }
		math::Vector getTexCoord() const { return m_texCoord; }

	protected:
		math::Vector m_texCoord;
	};

} /* namespace manta */

#endif /* BATCHED_MONTE_CARLO_EMITTER_GROUP_H */

