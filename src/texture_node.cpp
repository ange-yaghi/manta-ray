#include <texture_node.h>

#include <intersection_point.h>

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>

manta::TextureNode::TextureNode() {
	/* void */
}

manta::TextureNode::~TextureNode() {
	/* void */
}

void manta::TextureNode::loadFile(const char *fname, bool correctGamma) {
	m_defaultNode.setFilename(std::string(fname));
	m_defaultNode.setCorrectGamma(correctGamma);
}

void manta::TextureNode::registerInputs() {
	/* void */
}

void manta::TextureNode::registerOutputs() {
	registerOutput(&m_textureOutput, "Output");
}

void manta::TextureNode::_initialize() {
	m_defaultNode.initialize();
}

void manta::TextureNode::_evaluate() {
	m_defaultNode.evaluate();
	m_textureOutput.setMap(m_defaultNode.getMap());
}

void manta::TextureNode::_destroy() {
	m_defaultNode.destroy();
}
