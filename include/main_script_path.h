#ifndef MANTARAY_MAIN_SCRIPT_PATH_NODE_H
#define MANTARAY_MAIN_SCRIPT_PATH_NODE_H

#include "node.h"

namespace manta {

    class MainScriptPathNode : public Node {
    public:
        MainScriptPathNode();
        virtual ~MainScriptPathNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        piranha::LiteralStringOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_MAIN_SCRIPT_PATH_NODE_H */
