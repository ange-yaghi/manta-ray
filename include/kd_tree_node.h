#ifndef MANTARAY_KD_TREE_NODE_H
#define MANTARAY_KD_TREE_NODE_H

#include "object_reference_node.h"

namespace manta {

    class KdTreeNode : public ObjectReferenceNode<KDTree> {
    public:
        KdTreeNode();
        virtual ~KdTreeNode();

        KDTree *getKdTree() const { return m_kdTree; }

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_meshInput;
        piranha::pNodeInput m_granularityInput;
        piranha::pNodeInput m_widthInput;
        piranha::pNodeInput m_centerInput;
        piranha::pNodeInput m_cacheKeyInput;
        piranha::pNodeInput m_overwriteCacheInput;

    protected:
        KDTree *m_kdTree;
    };

} /* namespace manta */

#endif /* MANTARAY_KD_TREE_NODE_H */
