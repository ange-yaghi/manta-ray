#ifndef MANTARAY_ADD_BXDF_NODE_H
#define MANTARAY_ADD_BXDF_NODE_H

#include "node.h"

namespace manta {

    class AddBxdfNode : public Node {
    public:
        AddBxdfNode();
        ~AddBxdfNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();
        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::pNodeInput m_bsdfInput;
        piranha::pNodeInput m_bxdfInput;
    };

} /* namespace manta */

#endif /* MANTARAY_ADD_BXDF_NODE_H */
