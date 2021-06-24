#ifndef MANTARAY_COMPLEX_BAKE_NODE_H
#define MANTARAY_COMPLEX_BAKE_NODE_H

#include "object_reference_node.h"

#include "complex_map_2d.h"

namespace manta {

    class ComplexBakeNode : public ObjectReferenceNode<ComplexMap2D> {
    public:
        ComplexBakeNode();
        ~ComplexBakeNode();

        ComplexMap2D *getMap() { return &m_map; }

    protected:
        virtual void _evaluate();

    protected:
        ComplexMap2D m_map;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_BAKE_NODE_H */
