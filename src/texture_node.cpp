#include <texture_node.h>

#include <intersection_point.h>

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>

manta::TextureNode::TextureNode() {
	m_mapInputNode = nullptr;
}

manta::TextureNode::~TextureNode() {
	/* void */
}

void manta::TextureNode::loadFile(const char *fname, bool correctGamma) {
	m_mapInputNode = &m_defaultNode;

	m_defaultNode.setFilename(std::string(fname));
	m_defaultNode.setCorrectGamma(correctGamma);
}

manta::math::Vector manta::TextureNode::sample(const IntersectionPoint *surfaceInteraction) const {
	math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
	math::real v = math::getY(surfaceInteraction->m_textureCoodinates);

	// Wrap coordinates
	u = (math::real)fmod(u, (math::real)1.0);
	v = (math::real)fmod(v, (math::real)1.0);

	if (u < 0) u = 1 + u;
	if (v < 0) v = 1 + v;

	VectorMap2DNode *node = static_cast<VectorMap2DNode *>(m_mapInputNode);
	return node->getMap()->sample(u, 1 - v);
}

void manta::TextureNode::registerDependencies() {
	registerDependency(&m_mapInputNode, "Map");
}

void manta::TextureNode::_destroy() {
	VectorNode::_destroy();

	m_defaultNode.destroy();
}
