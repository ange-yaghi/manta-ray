#ifndef MANTARAY_DATE_INTERFACE_NODE_H
#define MANTARAY_DATE_INTERFACE_NODE_H

#include <piranha.h>

namespace manta {

    class DateInterfaceNode : public piranha::Node {
    public:
        DateInterfaceNode();
        virtual ~DateInterfaceNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();
        virtual void registerOutputs();

        piranha::LiteralStringOutput m_asStringOutput;

        piranha::pNodeInput m_dateInput;
    };

} /* namespace manta */

#endif /* MANTARAY_DATE_NODE_H */
