#include "../include/kd_tree_node.h"

#include "../include/mesh.h"

manta::KdTreeNode::KdTreeNode() {
    m_meshInput = nullptr;
}

manta::KdTreeNode::~KdTreeNode() {
    /* void */
}

void manta::KdTreeNode::_initialize() {
    /* void */
}

void manta::KdTreeNode::_evaluate() {
    m_kdTree.configure((math::real)1000.0, math::constants::Zero);

    Mesh *mesh = nullptr;
    mesh = ((ObjectReferenceNodeOutput<Mesh> *)m_meshInput)->getReference();

    m_kdTree.analyzeWithProgress(mesh, 2);
    
    m_output.setReference(&m_kdTree);
}

void manta::KdTreeNode::_destroy() {
    m_kdTree.destroy();
}

void manta::KdTreeNode::registerOutputs() {
    registerOutput(&m_output, "__out");
    setPrimaryOutput("__out");
}

void manta::KdTreeNode::registerInputs() {
    registerInput(&m_meshInput, "mesh");
}
