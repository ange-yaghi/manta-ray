#ifndef MANTARAY_KD_TREE_NODE_H
#define MANTARAY_KD_TREE_NODE_H

#include <piranha.h>

#include "scene_geometry_node_output.h"
#include "kd_tree.h"

namespace manta {

	class KdTreeNode : public piranha::Node {
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
		piranha::pNodeInput m_meshInput;
		SceneGeometryNodeOutput m_output;

	protected:
		KDTree m_kdTree;
	};

} /* namespace manta */

#endif /* MANTARAY_KD_TREE_NODE_H */
