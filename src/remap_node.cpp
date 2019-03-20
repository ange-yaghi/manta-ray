#include <remap_node.h>

#include <assert.h>

manta::RemapNode::RemapNode() {
	m_start = math::constants::One;
	m_end = math::constants::Zero;

	m_inputNode = nullptr;
}

manta::RemapNode::RemapNode(const math::Vector &start, const math::Vector &end, const VectorMaterialNode *inputNode) {
	m_start = start;
	m_end = end;

	m_inputNode = inputNode;
}

manta::RemapNode::~RemapNode() {

}

manta::math::Vector manta::RemapNode::sample(const IntersectionPoint *surfaceInteraction) const {
	assert(m_inputNode != nullptr);

	math::Vector raw = m_inputNode->sample(surfaceInteraction);
	math::Vector remapped = math::add(
		math::mul(m_start, math::sub(math::constants::One, raw)), 
		math::mul(m_end, raw));

	return remapped;
}
