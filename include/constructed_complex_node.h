#ifndef MANTARAY_CONSTRUCTED_COMPLEX_NODE_H
#define MANTARAY_CONSTRUCTED_COMPLEX_NODE_H

#include <piranha.h>

#include "constructed_complex_node_output.h"

namespace manta {

    class ConstructedComplexNode : public piranha::Node {
    public:
        ConstructedComplexNode();
        virtual ~ConstructedComplexNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        ConstructedComplexNodeOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_CONSTRUCTED_COMPLEX_NODE_H */
