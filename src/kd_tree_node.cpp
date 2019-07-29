#include "../include/kd_tree_node.h"

#include "../include/mesh.h"

manta::KdTreeNode::KdTreeNode() {
	/* void */
}

manta::KdTreeNode::~KdTreeNode() {
	/* void */
}

void manta::KdTreeNode::_initialize() {
	/* void */
}

void manta::KdTreeNode::_evaluate() {
	m_kdTree.initialize((math::real)1000.0, math::constants::Zero);

	Mesh *mesh = nullptr;
	mesh = ((ObjectReferenceNodeOutput<Mesh> *)m_meshInput)->getReference();

	m_kdTree.analyzeWithProgress(mesh, 2);
	
	m_output.setReference(&m_kdTree);
}

void manta::KdTreeNode::_destroy() {
	/* void */
}

void manta::KdTreeNode::registerOutputs() {
	registerOutput(&m_output, "__out");
	setPrimaryOutput(&m_output);
}

void manta::KdTreeNode::registerInputs() {
	registerInput(&m_meshInput, "mesh");
}
