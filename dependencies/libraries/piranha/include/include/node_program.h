#ifndef PIRANHA_NODE_PROGRAM_H
#define PIRANHA_NODE_PROGRAM_H

#include "node_container.h"
#include "pkey_value_lookup.h"
#include "ir_compilation_unit.h"
#include "language_rules.h"

#include <vector>

namespace piranha {

    class Node;
    class NodeAllocator;
    class IrParserStructure;
    class IrContextTree;

    class NodeProgram {
    public:
        static const bool CheckDuplicates = true;

    public:
        NodeProgram();
        ~NodeProgram();

        void initialize();
        void optimize();
        bool execute();
        void free();

        void writeAssembly(const std::string &fname) const;

        NodeContainer *getTopLevelContainer() { return &m_topLevelContainer; }

        int getNodeCount() const { return (int)m_nodeCache.size(); }

        void addNode(Node *node);
        Node *getCachedInstance(IrParserStructure *ir, IrContextTree *context);
        Node *getCachedInstance(Node *node);

        void addContainer(IrContextTree *context, NodeContainer *container);
        NodeContainer *getContainer(IrContextTree *context);

        std::string getRuntimeError() const { return m_errorMessage; }
        void throwRuntimeError(const std::string &msg, Node *node);

        void setRootUnit(IrCompilationUnit *unit) { m_rootUnit = unit; }
        IrCompilationUnit *getRootUnit() const { return m_rootUnit; }

        void setRootContext(IrContextTree *context) { m_rootContext = context; }
        IrContextTree *getRootContext() const { return m_rootContext; }

        NodeAllocator *getNodeAllocator() { return m_rootUnit->getRules()->getNodeAllocator(); }

    protected:
        bool m_initialized;

        IrContextTree *m_rootContext;
        IrCompilationUnit *m_rootUnit;

        NodeContainer m_topLevelContainer;
        PKeyValueLookup<IrContextTree, NodeContainer *> m_containers;

        std::vector<Node *> m_nodeCache;

        bool m_runtimeError;
        std::string m_errorMessage;
        Node *m_errorNode;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_PROGRAM_H */
