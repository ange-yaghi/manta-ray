#ifndef SIMPLE_SCATTER_EMITTER_H
#define SIMPLE_SCATTER_EMITTER_H

#include <ray_emitter.h>

namespace manta {

	class SimpleScatterEmitter : public RayEmitter {
	public:
		SimpleScatterEmitter();
		virtual ~SimpleScatterEmitter();

		virtual void generateRays();

		LightRay *getRay() const { return m_singleRay; }

	protected:
		LightRay *m_singleRay;
	};

}

#endif /* SCATTER_EMITTER_H */
