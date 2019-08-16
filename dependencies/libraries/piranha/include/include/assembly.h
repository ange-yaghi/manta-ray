#ifndef PIRANHA_ASSEMBLY_H
#define PIRANHA_ASSEMBLY_H

#include "key_value_lookup.h"

namespace piranha {

    class NodeOutput;

    class Assembly {
    public:
        Assembly();
        ~Assembly();

        int getOutputLabel(NodeOutput *output);

    protected:
        KeyValueLookup<NodeOutput *, int> m_outputLookup;
        int m_currentIndex;
    };

} /* namespace piranha */

#endif /* PIRANHA_ASSEMBLY_H */
