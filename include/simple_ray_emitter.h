#ifndef SIMPLE_RAY_EMITTER_H
#define SIMPLE_RAY_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class SimpleRayEmitter : public RayEmitter {
	public:
		SimpleRayEmitter();
		virtual ~SimpleRayEmitter();

		virtual void generateRays();

		LightRay *getRay() const { return m_singleRay; }

	protected:
		LightRay *m_singleRay;
	};

}

#endif /* SIMPLE_RAY_EMITTER_H */