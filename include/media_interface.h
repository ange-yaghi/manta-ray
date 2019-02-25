#ifndef MEDIA_INTERFACE_H
#define MEDIA_INTERFACE_H

#include <material_node.h>

#include <manta_math.h>

namespace manta {

	class MediaInterface : public MaterialNode {
	public:
		enum DIRECTION {
			DIRECTION_IN,
			DIRECTION_OUT
		};

	public:
		MediaInterface() {}
		~MediaInterface() {}

		virtual math::real fresnelTerm(const math::Vector &i, const math::Vector &m, DIRECTION d) const = 0;
		virtual math::real fresnelTerm(math::real cosThetaI, math::real *pdf, DIRECTION d) const = 0;

		virtual math::real ior(DIRECTION d) = 0;
		virtual math::real no(DIRECTION d) = 0;
		virtual math::real ni(DIRECTION d) = 0;
	};

} /* namespace manta */

#endif /* MEDIA_INTERFACE_H */
