#ifndef MANTARAY_MEDIA_INTERFACE_H
#define MANTARAY_MEDIA_INTERFACE_H

#include <piranha.h>

#include "manta_math.h"

namespace manta {

    class MediaInterface {
    public:
        enum DIRECTION {
            DIRECTION_IN,
            DIRECTION_OUT
        };

    public:
        MediaInterface();
        virtual ~MediaInterface();

        virtual math::real fresnelTerm(const math::Vector &i, const math::Vector &m, 
            DIRECTION d) const = 0;
        virtual math::real fresnelTerm(math::real cosThetaI, math::real *pdf, 
            DIRECTION d) const = 0;

        virtual math::real ior(DIRECTION d) const = 0;
        virtual math::real no(DIRECTION d) const = 0;
        virtual math::real ni(DIRECTION d) const = 0;
    };

} /* namespace manta */

#endif /* MANTARAY_MEDIA_INTERFACE_H */
