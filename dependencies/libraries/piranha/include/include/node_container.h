#ifndef PIRANHA_NODE_CONTAINER_H
#define PIRANHA_NODE_CONTAINER_H

#include "node.h"

#include <vector>
#include <fstream>

namespace piranha {

    class NodeContainer : public Node {
    public:
        NodeContainer();
        virtual ~NodeContainer();

        void addNode(Node *node);
        Node *getNode(int index) const { return m_nodes[index]; }
        bool findNode(Node *node) const;
        bool findContainer(Node *node) const;
        int getNodeCount() const { return (int)m_nodes.size(); }

        void addChild(NodeContainer *container);
        NodeContainer *getChild(int index) const { return m_children[index]; }
        int getChildCount() const { return (int)m_children.size(); }

        NodeContainer *getTopLevel();

        void addContainerInput(const std::string &name, bool modifiesInput, bool enableInput);
        void addContainerOutput(pNodeInput output, Node *node, const std::string &name, bool primary);

        virtual void writeAssembly(std::fstream &file, Assembly *assembly, int indent) const;

        void prune();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual Node *_optimize(NodeAllocator *nodeAllocator);

    protected:
        std::vector<NodeContainer *> m_children;
        std::vector<Node *> m_nodes;

        std::vector<pNodeInput *> m_connections;
    };

} /* namspace piranha */

#endif /* PIRANHA_NODE_CONTAINER_H */
