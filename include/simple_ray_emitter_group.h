#ifndef SIMPLE_RAY_EMITTER_GROUP_H
#define SIMPLE_RAY_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <simple_ray_emitter.h>

namespace manta {

	class SimpleRayEmitterGroup : public RayEmitterGroup {
	public:
		SimpleRayEmitterGroup();
		~SimpleRayEmitterGroup();

		virtual void createAllEmitters();

		SimpleRayEmitter *m_simpleRayEmitter;
	};

} /* namespace manta */

#endif /* SIMPLE_RAY_EMITTER_GROUP_H */
