#ifndef SDL_CONTEXT_TREE_H
#define SDL_CONTEXT_TREE_H

#include <vector>

namespace manta {

	class SdlNode;
	class SdlAttributeDefinition;
	class SdlParserStructure;
	class Node;
	class NodeOutput;

	class SdlContextTree {
	public:
		SdlContextTree();
		SdlContextTree(SdlNode *context, bool mainContext = false);
		~SdlContextTree();

		void setParent(SdlContextTree *parent) { m_parent = parent; }
		SdlContextTree *getParent() const { return m_parent; }
		SdlContextTree *getRoot();

		SdlContextTree *search(const SdlContextTree *reference) const;
		SdlContextTree *newChild(SdlNode *context, bool mainContext = false);

		SdlNode *getContext() const { return m_context; }
		SdlContextTree *findContext(SdlParserStructure *context);

		SdlParserStructure *resolveDefinition(SdlAttributeDefinition *definition);

		bool isMainContext() const { return m_mainContext; }

	protected:
		SdlNode *m_context;
		bool m_mainContext;

		SdlContextTree *m_parent;
		std::vector<SdlContextTree *> m_children;
	};

} /* namespace manta */

#endif /* SDL_CONTEXT_TREE_H */
