#ifndef BSDF_H
#define BSDF_H

#include <node.h>

#include <manta_math.h>
#include <media_interface.h>
#include <bsdf_node_output.h>

#include <algorithm>
#include <math.h>

namespace manta {

	struct IntersectionPoint;
	class StackAllocator;

	class BSDF : public Node {
	public:
		BSDF();
		~BSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const = 0;

		static inline bool refract(const math::Vector &i, const math::Vector &n, math::real ior, math::Vector *t) {
			math::real cosThetaI = math::getScalar(math::dot(n, i));
			math::real sin2ThetaI = std::max((math::real)0.0, (math::real)1.0 - cosThetaI * cosThetaI);
			math::real sin2ThetaT = ior * ior * sin2ThetaI;

			if (sin2ThetaT >= (math::real)1.0) return false;

			math::real cosThetaT = ::sqrt((math::real)1.0 - sin2ThetaT);

			math::Vector ior_v = math::loadScalar(ior);
			math::Vector S = math::loadScalar(ior * cosThetaI - cosThetaT);
			*t = math::add(
				math::mul(ior_v, math::negate(i)),
				math::mul(S, n)
			);

			return true;
		}

	protected:
		BSDFNodeOutput m_output;

		virtual void registerOutputs();
	};

} /* namespace manta */

#endif /* BSDF_H */
