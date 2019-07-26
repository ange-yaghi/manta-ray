#ifndef PIRANHA_NODE_OUTPUT_H
#define PIRANHA_NODE_OUTPUT_H

#include "channel_type.h"

#include <string>
#include <vector>

namespace piranha {

    struct IntersectionPoint;
    class Node;

    class NodeOutput {
    public:
        NodeOutput(const ChannelType *singleType);
        virtual ~NodeOutput();

        virtual void fullCompute(void *target) const { /* void */ }
        virtual void registerInputs() {}

        int getInputCount() const { return (int)m_inputs.size(); }
        NodeOutput **getInputConnection(int index) { return m_inputs[index]; }

        int getModifyConnectionCount() const;
        void addModifyConnection(Node *output);
        Node *getModifyConnection(int index) const;

        bool isType(const ChannelType &type) const { return m_singleType == &type; }
        const ChannelType *getType() const { return m_singleType; }

        void initialize();
        void evaluate();

        void setName(const std::string &name) { m_name = name; }
        const std::string &getName() const { return m_name; }

        void setParentNode(Node *parentNode) { m_parentNode = parentNode; }
        Node *getParentNode() const { return m_parentNode; }

        Node *getInterface() const { return m_interface; }

    protected:
        void overrideType(const ChannelType *type) { m_singleType = type; }

        virtual Node *generateInterface() { return nullptr; }

    private:
        const ChannelType *m_singleType;

        std::string m_name;

        Node *m_parentNode;
        Node *m_interface;

        bool m_evaluated;

    protected:
        virtual void registerInput(NodeOutput **nodeInput) { m_inputs.push_back(nodeInput); }

        std::vector<NodeOutput **> m_inputs;
        std::vector<Node *> m_modifyConnections;
    };

    // Type to reduce confusion
    typedef NodeOutput * pNodeInput;

} /* namespace piranha */

#endif /* PIRANHA_NODE_OUTPUT_H */
