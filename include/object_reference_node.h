#ifndef MANTARAY_OBJECT_REFERENCE_NODE_H
#define MANTARAY_OBJECT_REFERENCE_NODE_H

#include "node.h"

#include "object_reference_node_output.h"

namespace manta {

    template <typename Type>
    class ObjectReferenceNode : public Node {
    public:
        ObjectReferenceNode() {
            /* void */
        }

        ~ObjectReferenceNode() {
            /* void */
        }

    protected:
		virtual void registerOutputs() {
			setPrimaryOutput(&m_output);
			registerOutput(&m_output, "__out");
		}

        ObjectReferenceNodeOutput<Type> m_output;
    };

} /* namespace manta */

#endif /* MANTARAY_OBJECT_REFERENCE_NODE_H */
