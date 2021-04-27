#ifndef MANTARAY_OPAQUE_MEDIA_INTERFACE_H
#define MANTARAY_OPAQUE_MEDIA_INTERFACE_H

#include "media_interface.h"

namespace manta {

    class OpaqueMediaInterface : public MediaInterface {
    public:
        OpaqueMediaInterface();
        virtual ~OpaqueMediaInterface();

        virtual math::real fresnelTerm(
            const math::Vector &i,
            const math::Vector &m,
            Direction d) const { return (math::real)1.0; }
        virtual math::real fresnelTerm(math::real cosThetaI, Direction d) const { return (math::real)1.0; }

        virtual math::real ior(Direction d) const { return (math::real)1.0; }
        virtual math::real no(Direction d) const { return (math::real)1.0; }
        virtual math::real ni(Direction d) const { return (math::real)1.0; }
    };

} /* namespace manta */

#endif /* MANTARAY_OPAQUE_MEDIA_INTERFACE */
