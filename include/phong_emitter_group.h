#ifndef PHONG_EMITTER_GROUP_H
#define PHONG_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class PhongEmitter;

	class PhongEmitterGroup : public RayEmitterGroup {
	public:
		PhongEmitterGroup();
		virtual ~PhongEmitterGroup();

		virtual void createAllEmitters();

		PhongEmitter *getPhongEmitter(int index) const { return (PhongEmitter *)m_rayEmitters[index]; }
	};

} /* namespace manta */

#endif /* PHONG_EMITTER_GROUP_H */
