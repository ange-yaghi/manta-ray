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

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const = 0;
	};

} /* namespace manta */

#endif /* BSDF_H */
