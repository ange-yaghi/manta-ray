#include "..\include\mesh_merge_node.h"

manta::MeshMergeNode::MeshMergeNode() {
    m_leftMesh = nullptr;
    m_rightMesh = nullptr;
}

manta::MeshMergeNode::~MeshMergeNode() {
    /* void */
}

void manta::MeshMergeNode::_initialize() {
    /* void */
}

void manta::MeshMergeNode::_evaluate() {
    Mesh *leftMesh = getObject<Mesh>(m_leftMesh);
    Mesh *rightMesh = getObject<Mesh>(m_rightMesh);

    leftMesh->merge(rightMesh);

    m_output.setReference(leftMesh);
}

void manta::MeshMergeNode::registerInputs() {
    registerInput(&m_leftMesh, "__left", true);
    registerInput(&m_rightMesh, "__right");
}

void manta::MeshMergeNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}
