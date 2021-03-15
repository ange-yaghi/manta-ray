#ifndef PIRANHA_NODE_OUTPUT_H
#define PIRANHA_NODE_OUTPUT_H

#include "channel_type.h"

#include <string>
#include <vector>

namespace piranha {

    struct IntersectionPoint;
    class Node;
    class NodeAllocator;

    class NodeOutput {
    public:
        NodeOutput(const ChannelType *singleType);
        virtual ~NodeOutput();

        virtual void fullCompute(void *target) const { /* void */ }
        virtual void registerInputs() { /* void */ }

        int getInputCount() const { return (int)m_inputs.size(); }
        NodeOutput **getInputConnection(int index) { return m_inputs[index]; }

        int getModifyConnectionCount() const;
        void addModifyConnection(Node *output);
        Node *getModifyConnection(int index) const;

        bool isType(const ChannelType &type) const { return m_singleType == &type; }
        const ChannelType *getType() const { return m_singleType; }

        void initialize();
        bool evaluate();
        bool checkEnabled();

        bool isEnabled() const { return m_enabled; }

        void setName(const std::string &name) { m_name = name; }
        const std::string &getName() const { return m_name; }

        void setParentNode(Node *parentNode) { m_parentNode = parentNode; }
        Node *getParentNode() const { return m_parentNode; }

        Node *generateInterface(NodeAllocator *nodeAllocator);
        Node *getInterface() const;

        void overrideType(const ChannelType *type) { m_singleType = type; }

        void addDependency(Node *node) { m_dependencyChain.push_back(node); }

    protected:
        virtual Node *newInterface(NodeAllocator *nodeAllocator) { return nullptr; }

    private:
        const ChannelType *m_singleType;

        std::string m_name;

        Node *m_parentNode;
        Node *m_interface;

        bool m_evaluated;
        bool m_evaluating;
        bool m_checkedEnabled;

        bool m_enabled;

    protected:
        virtual void _evaluate();
        virtual void registerInput(NodeOutput **nodeInput) { m_inputs.push_back(nodeInput); }

        std::vector<NodeOutput **> m_inputs;
        std::vector<Node *> m_modifyConnections;
        std::vector<Node *> m_dependencyChain;
    };

    // Type to reduce confusion
    typedef NodeOutput * pNodeInput;

} /* namespace piranha */

#endif /* PIRANHA_NODE_OUTPUT_H */
