#ifndef MANTARAY_FRESNEL_NODE_H
#define MANTARAY_FRESNEL_NODE_H

#include <piranha.h>

#include "fresnel_node_output.h"

namespace manta {

    class FresnelNode : public piranha::Node {
    public:
        FresnelNode();
        virtual ~FresnelNode();

        FresnelNodeOutput *getMainOutput() { return &m_output; }

    protected:
        FresnelNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_FRESNEL_NODE_H */
