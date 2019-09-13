#ifndef MANTARAY_SCRIPT_PATH_NODE_H
#define MANTARAY_SCRIPT_PATH_NODE_H

#include "node.h"

namespace manta {

    class ScriptPathNode : public Node {
    public:
        ScriptPathNode();
        virtual ~ScriptPathNode();

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

#endif /* MANTARAY_SCRIPT_PATH_NODE_H */
