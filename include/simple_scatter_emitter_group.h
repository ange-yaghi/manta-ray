#ifndef SCATTER_EMITTER_GROUP_H
#define SCATTER_EMITTER_GROUP_H

#include <ray_emitter_group.h>

namespace manta {

	class SimpleScatterEmitter;

	class SimpleScatterEmitterGroup : public RayEmitterGroup {
	public:
		SimpleScatterEmitterGroup();
		~SimpleScatterEmitterGroup();

		virtual void createAllEmitters();

		SimpleScatterEmitter *m_simpleRayEmitter;
	};

} /* namespace manta */

#endif /* SIMPLE_SCATTER_EMITTER_GROUP_H */
