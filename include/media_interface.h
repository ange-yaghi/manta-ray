#ifndef MANTARAY_MEDIA_INTERFACE_H
#define MANTARAY_MEDIA_INTERFACE_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta {

    class MediaInterface : public ObjectReferenceNode<MediaInterface> {
    public:
        enum class Direction {
            In,
            Out
        };

    public:
        MediaInterface();
        virtual ~MediaInterface();

        virtual math::real fresnelTerm(const math::Vector &i, const math::Vector &m, 
            Direction d) const = 0;
        virtual math::real fresnelTerm(math::real cosThetaI, math::real *pdf, 
            Direction d) const = 0;

        virtual math::real ior(Direction d) const = 0;
        virtual math::real no(Direction d) const = 0;
        virtual math::real ni(Direction d) const = 0;

    protected:
        virtual void _evaluate();
    };

} /* namespace manta */

#endif /* MANTARAY_MEDIA_INTERFACE_H */
