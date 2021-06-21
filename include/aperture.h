#ifndef MANTARAY_APERTURE_H
#define MANTARAY_APERTURE_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta {

    class Aperture : public ObjectReferenceNode<Aperture> {
    public:
        Aperture();
        virtual ~Aperture();

        virtual bool isNotBlocked(math::real x, math::real y) const = 0;
        math::real sample(math::real x, math::real y);

        void setRadius(math::real size) { m_radius = size; };
        math::real getRadius() const { return m_radius; };

    protected:
        math::real m_radius;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::pNodeInput m_dirt;
        piranha::pNodeInput m_radiusInput;
    };

} /* namespace manta */

#endif /* MANTARAY_APERTURE_H */
