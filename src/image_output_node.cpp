#include <image_output_node.h>

#include <image_byte_buffer.h>
#include <map_2d_node.h>
#include <jpeg_writer.h>

manta::ImageOutputNode::ImageOutputNode() {
	m_outputFilename = "";
	m_gammaCorrection = false;
	m_inputNode = nullptr;
	m_jpegQuality = 95;
}

manta::ImageOutputNode::~ImageOutputNode() {
	/* void */
}

void manta::ImageOutputNode::_initialize() {
	/* void */
}

void manta::ImageOutputNode::_evaluate() {
	VectorMap2DNode *inputNode = static_cast<VectorMap2DNode *>(m_inputNode);

	ImageByteBuffer byteBuffer;
	inputNode->getMap()->fillByteBuffer(&byteBuffer, m_gammaCorrection);

	JpegWriter jpegWriter;
	jpegWriter.setQuality(m_jpegQuality);
	jpegWriter.write(&byteBuffer, m_outputFilename.c_str());

	byteBuffer.free();
}

void manta::ImageOutputNode::_destroy() {
	/* void */
}

void manta::ImageOutputNode::registerDependencies() {
	registerDependency(&m_inputNode, "Input");
}
