#ifndef PIRANHA_IR_CONTEXT_TREE_H
#define PIRANHA_IR_CONTEXT_TREE_H

#include <vector>

namespace piranha {

    class IrNode;
    class IrAttributeDefinition;
    class IrParserStructure;
    class Node;
    class NodeOutput;

    class IrContextTree {
    public:
        IrContextTree();
        IrContextTree(IrNode *context, bool mainContext = false);
        ~IrContextTree();

        void setParent(IrContextTree *parent) { m_parent = parent; }
        IrContextTree *getParent() const { return m_parent; }
        IrContextTree *getRoot();
        IrContextTree *getMain();

        IrContextTree *newChild(IrNode *context, bool mainContext = false);

        IrNode *getContext() const { return m_context; }
        bool hasParent(const IrContextTree *context) const;
        IrContextTree *findContext(IrNode *context);

        IrParserStructure *resolveDefinition(IrAttributeDefinition *definition);

        bool isMainContext() const { return m_mainContext; }

        bool operator==(const IrContextTree &ref) const { return isEqual(&ref); }
        bool isEqual(const IrContextTree *ref) const;
        bool isOutside(const IrContextTree *ref) const;

        int getChildCount() const { return (int)m_children.size(); }

        bool isEmpty() const { return m_context == nullptr; }

        virtual void free();

    protected:
        IrContextTree *_getMain();

        IrNode *m_context;
        bool m_mainContext;

        IrContextTree *m_parent;
        std::vector<IrContextTree *> m_children;
    };

} /* namespace piranha */

#endif /* PIRANHA_IR_CONTEXT_TREE_H */
