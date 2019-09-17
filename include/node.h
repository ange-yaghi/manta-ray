#ifndef MANTARAY_NODE_H
#define MANTARAY_NODE_H

#include <piranha.h>

#include "stack_allocator.h"

namespace manta {

    struct IntersectionPoint;
    class Path;

    class Node : public piranha::Node {
    public:
        Node();
        virtual ~Node();

        void resolvePath(const Path *relative, Path *target) const;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();
    };

} /* namespace manta */

#endif /* MANTARAY_NODE_H */
