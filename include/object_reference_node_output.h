#ifndef MANTARAY_OBJECT_REFERENCE_NODE_OUTPUT_H
#define MANTARAY_OBJECT_REFERENCE_NODE_OUTPUT_H

#include <piranha.h>

#include "object_channel_types.h"

namespace manta {

    template <typename Type>
    class ObjectReferenceNodeOutput : public piranha::NodeOutput {
    public:
        ObjectReferenceNodeOutput() : NodeOutput(LookupChannelType<Type>()) {
            /* void */
        }

        virtual ~ObjectReferenceNodeOutput() {
            /* void */
        }

        Type *getReference() const { return m_data; }
        void setReference(Type *data) { m_data = data; }

    protected:
        Type *m_data;
    };

    template <typename Type>
    Type *getObject(piranha::pNodeInput input) {
        return static_cast<ObjectReferenceNodeOutput<Type> *>(input)->getReference();
    }

} /* namespace manta */

#endif /* MANTARAY_OBJECT_REFERENCE_NODE_OUTPUT_H */
