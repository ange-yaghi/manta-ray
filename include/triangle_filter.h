#ifndef MANTARAY_TRIANGLE_FILTER_H
#define MANTARAY_TRIANGLE_FILTER_H

#include "filter.h"

namespace manta {

    class TriangleFilter : public Filter {
    public:
        TriangleFilter();
        ~TriangleFilter();

        virtual math::Vector evaluate(const math::Vector2 &p);
    };

} /* namespace manta */

#endif /* MANTARAY_TRIANGLE_FILTER_H */

