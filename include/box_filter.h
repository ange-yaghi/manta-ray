#ifndef MANTARAY_BOX_FILTER_H
#define MANTARAY_BOX_FILTER_H

#include "filter.h"

namespace manta {

    class BoxFilter : public Filter {
    public:
        BoxFilter();
        ~BoxFilter();

        virtual math::Vector evaluate(const math::Vector2 &p);
    };

} /* namespace manta */

#endif /* MANTARAY_BOX_FILTER_H */
