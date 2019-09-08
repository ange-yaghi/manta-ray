#ifndef PIRANHA_NODE_GRAPH_H
#define PIRANHA_NODE_GRAPH_H

#include <vector>

namespace piranha {

    class Node;
    class NodeProgram;
    class NodeContainer;

    class NodeGraph {
    public:
        struct GraphNode {
            Node *node;
            std::vector<GraphNode *> outConnections;
            std::vector<GraphNode *> inConnections;

            bool hasOutConnection(GraphNode *node);
            bool hasInConnection(GraphNode *node);

            int liveOutputConnections;
        };

    public:
        NodeGraph();
        ~NodeGraph();

        void generateNodeGraph(NodeProgram *program);

        void markDeadTree(GraphNode *node);
        void markDeadNodes();

        int getNodeCount() const { return (int)m_nodes.size(); }

    protected:
        GraphNode *findNode(Node *node);
        void createNode(Node *node);

        void generateNodes(NodeContainer *container);
        void generateConnections();

        std::vector<GraphNode *> m_nodes;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_GRAPH_H */
