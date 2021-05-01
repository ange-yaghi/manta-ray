#ifndef MANTARAY_HABLE_FILMIC_NODE_H
#define MANTARAY_HABLE_FILMIC_NODE_H

#include "node.h"

#include "hable_filmic_node_output.h"

namespace manta {

    class HableFilmicNode : public Node {
    public:
        HableFilmicNode();
        virtual ~HableFilmicNode();

        HableFilmicNodeOutput *getMainOutput() { return &m_output; }

    protected:
        HableFilmicNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_HABLE_FILMIC_NODE_H */
