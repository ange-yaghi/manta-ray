#ifndef MONTE_CARLO_SPECULAR_DIFFUSE_GROUP_H
#define MONTE_CARLO_SPECULAR_DIFFUSE_GROUP_H

#include <ray_emitter_group.h>

#include <simple_monte_carlo_emitter.h>
#include <simple_ray_emitter.h>

namespace manta {

	class MonteCarloSpecularDiffuseGroup : public RayEmitterGroup {
	public:
		MonteCarloSpecularDiffuseGroup();
		virtual ~MonteCarloSpecularDiffuseGroup();

		virtual void createAllEmitters();

		SimpleMonteCarloEmitter *m_diffuseEmitter;
		SimpleRayEmitter *m_specularEmitter;

		void setDiffuseEnabled(bool enabled) { m_diffuseEnabled = enabled; }
		bool isDiffuseEnabled() const { return m_diffuseEnabled; }

		void setSpecularEnabled(bool enabled) { m_specularEnabled = enabled; }
		bool isSpecularEnabled() const { return m_specularEnabled; }

	protected:
		bool m_diffuseEnabled;
		bool m_specularEnabled;
	};

} /* namespace manta */

#endif /* MONTE_CARLO_SPECULAR_DIFFUSE_H */
