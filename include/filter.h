#ifndef MANTARAY_FILTER_H
#define MANTARAY_FILTER_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta{

    class Filter : public ObjectReferenceNode<Filter> {
    public:
        Filter();
        ~Filter();
        
        virtual math::Vector evaluate(const math::Vector2 &p) = 0;

        void setExtents(const math::Vector2 &extents) { m_extents = extents; }
        math::Vector2 getExtents() const { return m_extents; }

    protected:
        math::Vector2 m_extents;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_radiusInput;
    };

} /* namespace manta */

#endif /* MANTARAY_FILTER_H */
