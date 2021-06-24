#ifndef MANTARAY_COMPLEX_MAP_2D_NODE_H
#define MANTARAY_COMPLEX_MAP_2D_NODE_H

#include <piranha.h>

#include "complex_map_2d.h"
#include "complex_map_2d_node_output.h"

namespace manta {

    class ComplexMap2dNode : public piranha::Node {
    public:
        ComplexMap2dNode();
        virtual ~ComplexMap2dNode();

        ComplexMap2D *getMap() { return &m_map; }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

    protected:
        ComplexMap2D m_map;

        ComplexMap2dNodeOutput m_output;

        piranha::pNodeInput m_width;
        piranha::pNodeInput m_height;
        piranha::pNodeInput m_input;
    };

} /* namespace manta */

#endif /* MANTARAY_COMPLEX_MAP_2D_NODE_H */
