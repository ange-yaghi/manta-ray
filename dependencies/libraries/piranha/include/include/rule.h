#ifndef PIRANHA_RULE_H
#define PIRANHA_RULE_H

#include "memory_tracker.h"

#include <string>

namespace piranha {

    class Node;

    template <typename ValueType, typename NodeBase>
    class Rule {
    public:
        Rule(const ValueType &value) {
            m_value = value;
            m_reference = nullptr;
        }

        Rule() {
            m_reference = nullptr;
        }

        virtual ~Rule() {
            /* void */
        }

        virtual NodeBase *buildNode(NodeAllocator *nodeAllocator) const = 0;
        void destroy(NodeAllocator *nodeAllocator) { nodeAllocator->free(m_reference); }

        void setValue(const ValueType &value) { m_value = value; }
        const ValueType &getValue() const { return m_value; }

        void setReference(NodeBase *reference) {
            m_reference = reference;
        }

        const NodeBase *getReference() const {
            return m_reference;
        }

        void generateReference(NodeAllocator *nodeAllocator) {
            NodeBase *reference = buildNode(nodeAllocator);
            reference->initialize();

            Rule<ValueType, NodeBase>::setReference(reference);
        }

    protected:
        ValueType m_value;
        NodeBase *m_reference;
    };

    template <typename ValueType, typename NodeType, typename NodeBase>
    class SpecializedRule : public Rule<ValueType, NodeBase> {
    public:
        SpecializedRule(const ValueType &value) : Rule<ValueType, NodeBase>(value) {
            /* void */
        }

        SpecializedRule() : Rule<ValueType, NodeBase>() {
            /* void */
        }

        virtual ~SpecializedRule() {
            /* void */
        }

        virtual NodeBase *buildNode(NodeAllocator *nodeAllocator) const {
            return nodeAllocator->allocate<NodeType>();
        }
    };

} /* namespace piranha */

#endif /* PIRANHA_RULE_H */
