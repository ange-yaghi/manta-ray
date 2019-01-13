#ifndef HEMI_MONTE_CARLO_EMITTER_GROUP_H
#define HEMI_MONTE_CARLO_EMITTER_GROUP_H

#include <ray_emitter_group.h>

#include <manta_math.h>

namespace manta {

	class HemiMonteCarloEmitter;

	class HemiMonteCarloEmitterGroup : public RayEmitterGroup {
	public:
		HemiMonteCarloEmitterGroup();
		virtual ~HemiMonteCarloEmitterGroup();

		virtual void createAllEmitters();

		void setTexCoord(const math::Vector &texCoord) { m_texCoord = texCoord; }
		math::Vector getTexCoord() const { return m_texCoord; }

		void setLayerCount(int layers) { m_layerCount = layers; }
		int getLayercount() const { return m_layerCount; }

		HemiMonteCarloEmitter *getHemiEmitter(int index) const { return (HemiMonteCarloEmitter *)m_rayEmitters[index]; }

	protected:
		math::Vector m_texCoord;

		int m_layerCount;
	};

} /* namespace manta */

#endif /* HEMI_MONTE_CARLO_EMITTER_GROUP_H */
