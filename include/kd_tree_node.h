#ifndef KD_TREE_NODE_H
#define KD_TREE_NODE_H

#include <node.h>

#include <scene_geometry_node_output.h>
#include <kd_tree.h>

namespace manta {

	class KdTreeNode : public Node {
	public:
		KdTreeNode();
		virtual ~KdTreeNode();

		KDTree *getKdTree() { return &m_kdTree; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		pNodeInput m_meshInput;
		SceneGeometryNodeOutput m_output;

	protected:
		KDTree m_kdTree;
	};

} /* namespace manta */

#endif /* KD_TREE_NODE_H */
