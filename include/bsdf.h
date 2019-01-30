#ifndef BSDF_H
#define BSDF_H

#include <manta_math.h>
#include <media_interface.h>

namespace manta {

	class BSDF {
	public:
		BSDF();
		~BSDF();

		virtual math::Vector generateMicrosurfaceNormal(const math::Vector &normal, const math::Vector &incident, const math::Vector &u, const math::Vector &v) const = 0;
		virtual math::real generateWeight(const math::Vector &normal, const math::Vector &incident, const math::Vector &m, const math::Vector &o) const = 0;
		virtual math::real bidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const;

		virtual MediaInterface::DIRECTION decideDirection(const math::Vector &incident, const math::Vector &m, MediaInterface::DIRECTION direction) const;

		math::Vector transmissionDirection(math::real ior, const math::Vector &incident, const math::Vector &m, const math::Vector &n) const;
		math::Vector reflectionDirection(const math::Vector &incident, const math::Vector &m) const;

		void setMediaInterface(const MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
		const MediaInterface *getMediaInterface() const { return m_mediaInterface; }

	protected:
		math::real smithBidirectionalShadowMasking(const math::Vector &normal, const math::Vector &incident, const math::Vector &o, const math::Vector &m) const;
		virtual math::real g1(const math::Vector &normal, const math::Vector &incident, const math::Vector &v, const math::Vector &m) const = 0;

		const MediaInterface *m_mediaInterface;
	};

} /* namespace manta */

#endif /* BSDF_H */
