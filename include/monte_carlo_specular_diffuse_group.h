#ifndef MONTE_CARLO_SPECULAR_DIFFUSE_GROUP_H
#define MONTE_CARLO_SPECULAR_DIFFUSE_GROUP_H

#include <ray_emitter_group.h>

#include <batched_monte_carlo_emitter.h>
#include <simple_ray_emitter.h>

namespace manta {

	class MonteCarloSpecularDiffuseGroup : public RayEmitterGroup {
	public:
		MonteCarloSpecularDiffuseGroup();
		virtual ~MonteCarloSpecularDiffuseGroup();

		virtual void createAllEmitters();

		BatchedMonteCarloEmitter *m_diffuseEmitter;
		BatchedMonteCarloEmitter *m_specularEmitter;

		void setDiffuseEnabled(bool enabled) { m_diffuseEnabled = enabled; }
		bool isDiffuseEnabled() const { return m_diffuseEnabled; }

		void setSpecularEnabled(bool enabled) { m_specularEnabled = enabled; }
		bool isSpecularEnabled() const { return m_specularEnabled; }

		void setDiffuseSamples(int samples) { m_diffuseSamples = samples; }
		int getDiffuseSamples() const { return m_diffuseSamples; }

		void setTexCoord(const math::Vector &texCoord) { m_texCoord = texCoord; }
		math::Vector getTexCoord() const { return m_texCoord; }

	protected:
		bool m_diffuseEnabled;
		bool m_specularEnabled;

		math::Vector m_texCoord;

		int m_diffuseSamples;
	};

} /* namespace manta */

#endif /* MONTE_CARLO_SPECULAR_DIFFUSE_H */
