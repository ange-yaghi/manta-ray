#ifndef PIRANHA_NODE_PROGRAM_H
#define PIRANHA_NODE_PROGRAM_H

#include "node_container.h"
#include "pkey_value_lookup.h"

#include <vector>

namespace piranha {

    class Node;
    class LanguageRules;
    class IrParserStructure;
    class IrContextTree;

    class NodeProgram {
    public:
        NodeProgram();
        ~NodeProgram();

        bool execute();

        void writeAssembly(const std::string &fname) const;

        NodeContainer *getTopLevelContainer() { return &m_topLevelContainer; }

        int getNodeCount() const { return (int)m_nodeCache.size(); }

        void addNode(Node *node);
        Node *getCachedInstance(IrParserStructure *ir, IrContextTree *context);

        void addContainer(IrContextTree *context, NodeContainer *container);
        NodeContainer *getContainer(IrContextTree *context);

        std::string getRuntimeError() const { return m_errorMessage; }
        void throwRuntimeError(const std::string &msg, Node *node);

    protected:
        NodeContainer m_topLevelContainer;
        PKeyValueLookup<IrContextTree, NodeContainer *> m_containers;

        std::vector<Node *> m_nodeCache;

        bool m_runtimeError;
        std::string m_errorMessage;
        Node *m_errorNode;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_PROGRAM_H */
