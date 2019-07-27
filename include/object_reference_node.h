#ifndef MANTARAY_OBJECT_REFERENCE_NODE_H
#define MANTARAY_OBJECT_REFERENCE_NODE_H

#include <piranha.h>

#include "object_reference_node_output.h"

namespace manta {

    template <typename Type>
    class ObjectReferenceNode : piranha::Node {
    public:
        ObjectReferenceNode(const piranha::ChannelType *channelType)
            : m_output(channelType)
        {
            /* void */
        }

        ~ObjectReferenceNode() {
            /* void */
        }

    protected:
        virtual void _initialize();
        virtual void registerOutputs();

        ObjectReferenceNodeOutput<Type> m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_OBJECT_REFERENCE_NODE_H */
