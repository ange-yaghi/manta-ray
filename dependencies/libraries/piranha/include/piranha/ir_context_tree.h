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

		IrContextTree *search(const IrContextTree *reference) const;
		IrContextTree *newChild(IrNode *context, bool mainContext = false);

		IrNode *getContext() const { return m_context; }
		IrContextTree *findContext(IrParserStructure *context);

		IrParserStructure *resolveDefinition(IrAttributeDefinition *definition);

		bool isMainContext() const { return m_mainContext; }

		bool isEqual(const IrContextTree *ref) const;

	protected:
		IrNode *m_context;
		bool m_mainContext;

		IrContextTree *m_parent;
		std::vector<IrContextTree *> m_children;
	};

} /* namespace piranha */

#endif /* PIRANHA_IR_CONTEXT_TREE_H */
