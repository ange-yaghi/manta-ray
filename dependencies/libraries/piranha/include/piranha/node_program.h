#ifndef PIRANHA_NODE_PROGRAM_H
#define PIRANHA_NODE_PROGRAM_H

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
        void execute();

        int getNodeCount() const { return (int)m_nodes.size();  }
        void addNode(Node *node);
        Node *getNode(int index) const { return m_nodes[index]; }

        void setRules(LanguageRules *rules) { m_rules = rules; }
        LanguageRules *getRules() { return m_rules; }

        Node *getCachedInstance(IrParserStructure *ir, IrContextTree *context);

        void writeAssembly(const std::string &fname) const;

    protected:
        std::vector<Node *> m_nodes;

        LanguageRules *m_rules;
    };

} /* namespace piranha */

#endif /* PIRANHA_NODE_PROGRAM_H */
