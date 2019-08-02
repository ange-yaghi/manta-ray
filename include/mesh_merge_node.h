#ifndef MANTARAY_MESH_MERGE_NODE_H
#define MANTARAY_MESH_MERGE_NODE_H

#include "node.h"

#include "object_reference_node_output.h"
#include "mesh.h"

namespace manta {

    class MeshMergeNode : public Node {
    public:
        MeshMergeNode();
        virtual ~MeshMergeNode();

    protected:
        piranha::pNodeInput m_leftMesh;
        piranha::pNodeInput m_rightMesh;
        ObjectReferenceNodeOutput<Mesh> m_output;

        virtual void _initialize();
        virtual void _evaluate();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_MESH_MERGE_NODE_H */
