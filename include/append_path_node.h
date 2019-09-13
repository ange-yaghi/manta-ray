#ifndef MANTARAY_APPEND_PATH_NODE_H
#define MANTARAY_APPEND_PATH_NODE_H

#include "node.h"

namespace manta {

    class AppendPathNode : public Node {
    public:
        AppendPathNode();
        virtual ~AppendPathNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_basePath;
        piranha::pNodeInput m_path;

        piranha::LiteralStringOutput m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_APPEND_PATH_NODE_H */

