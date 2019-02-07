#ifndef BSDF_H
#define BSDF_H

#include <material_node.h>

#include <manta_math.h>
#include <media_interface.h>

namespace manta {

	struct IntersectionPoint;
	class StackAllocator;

	struct BSDFInput {
		math::Vector normal;
		math::Vector incident;

		math::Vector u;
		math::Vector v;

		const IntersectionPoint *surfaceInteraction;

		unsigned char cachedParameters[64];
		void *extraMemory;
	};

	class BSDF : public MaterialNode {
	public:
		BSDF();
		~BSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const { return math::Vector(); }

		virtual math::Vector generateMicrosurfaceNormal(const BSDFInput &bsdfInput) const { return math::Vector(); }
		virtual math::real generateWeight(const BSDFInput &bsdfInput, const math::Vector &m, const math::Vector &o) const { return math::real(); }
		virtual math::real bidirectionalShadowMasking(const BSDFInput &bsdfInput, const math::Vector &o, const math::Vector &m) const;

		virtual MediaInterface::DIRECTION decideDirection(const BSDFInput &bsdfInput, const math::Vector &m, MediaInterface::DIRECTION direction) const;

		math::Vector transmissionDirection(const BSDFInput &bsdfInput, math::real ior, const math::Vector &m) const;
		math::Vector reflectionDirection(const BSDFInput &bsdfInput, const math::Vector &m) const;

		void setMediaInterface(const MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
		const MediaInterface *getMediaInterface() const { return m_mediaInterface; }

	protected:
		math::real smithBidirectionalShadowMasking(const BSDFInput &bsdfInput, const math::Vector &o, const math::Vector &m) const;
		virtual math::real g1(const BSDFInput &bsdfInput, const math::Vector &v, const math::Vector &m) const { return 0.0; }

		const MediaInterface *m_mediaInterface;
	};

} /* namespace manta */

#endif /* BSDF_H */
