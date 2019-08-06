#ifndef MANTARAY_CURRENT_DATE_NODE_H
#define MANTARAY_CURRENT_DATE_NODE_H

#include <piranha.h>

#include "date_node_output.h"

namespace manta {

    class CurrentDateNode : public piranha::Node {
    public:
        CurrentDateNode();
        virtual ~CurrentDateNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();

        DateNodeOutput m_dateOutput;
    };

} /* namespace manta */

#endif /* MANTARAY_CURRENT_DATE_NODE_H */
