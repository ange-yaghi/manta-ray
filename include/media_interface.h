#ifndef MEDIA_INTERFACE_H
#define MEDIA_INTERFACE_H

#include <manta_math.h>

namespace manta {

	class MediaInterface {
	public:
		enum DIRECTION {
			DIRECTION_IN,
			DIRECTION_OUT
		};

	public:
		MediaInterface() {}
		~MediaInterface() {}

		virtual math::real fresnelTerm(const math::Vector &i, const math::Vector &m, DIRECTION d) const = 0;
	};

} /* namespace manta */

#endif /* MEDIA_INTERFACE_H */
