#ifndef NODE_PROGRAM_H
#define NODE_PROGRAM_H

#include <vector>

#include <material_manager.h>

namespace manta {

	class Node;

	class NodeProgram {
	public:
		NodeProgram();
		~NodeProgram();

		int getNodeCount() const { return (int)m_nodes.size();  }
		void addNode(Node *node);
		Node *getNode(int index) const { return m_nodes[index]; }

		void execute();

		MaterialManager *getMaterialManager() { return &m_materialManager; }

	protected:
		std::vector<Node *> m_nodes;

		// High-level program context
		MaterialManager m_materialManager;
	};

} /* namespace manta */

#endif /* NODE_PROGRAM_H */
