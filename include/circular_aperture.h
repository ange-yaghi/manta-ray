#ifndef MANTARAY_CIRCULAR_APERTURE_H
#define MANTARAY_CIRCULAR_APERTURE_H

#include "aperture.h"

namespace manta {

    class CircularAperture : public Aperture {
    public:
        CircularAperture();
        virtual ~CircularAperture();

        virtual bool isNotBlocked(math::real x, math::real y) const;
    };

} /* namespace manta */

#endif /* MANTARAY_CIRCULAR_APERTURE_H */
