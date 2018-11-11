#ifndef SIMPLE_MONTE_CARLO_EMITTER_GROUP_H
#define SIMPLE_MONTE_CARLO_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <simple_monte_carlo_emitter.h>

namespace manta {

	class SimpleMonteCarloEmitterGroup : public RayEmitterGroup {
	public:
		SimpleMonteCarloEmitterGroup();
		~SimpleMonteCarloEmitterGroup();

		virtual void createAllEmitters();

		SimpleMonteCarloEmitter *m_simpleRayEmitter;
	};

} /* namespace manta */

#endif /* SIMPLE_MONTE_CARLO_EMITTER_GROUP_H */
