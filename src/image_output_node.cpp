#include <image_output_node.h>

#include <image_byte_buffer.h>
#include <jpeg_writer.h>

manta::ImageOutputNode::ImageOutputNode() {
	m_outputFilename = "";
	m_gammaCorrection = false;
	m_jpegQuality = 95;
}

manta::ImageOutputNode::~ImageOutputNode() {
	/* void */
}

void manta::ImageOutputNode::_initialize() {
	/* void */
}

void manta::ImageOutputNode::_evaluate() {
	ImageByteBuffer byteBuffer;
	m_input->getMap()->fillByteBuffer(&byteBuffer, m_gammaCorrection);

	JpegWriter jpegWriter;
	jpegWriter.setQuality(m_jpegQuality);
	jpegWriter.write(&byteBuffer, m_outputFilename.c_str());

	byteBuffer.free();
}

void manta::ImageOutputNode::_destroy() {
	/* void */
}

void manta::ImageOutputNode::registerInputs() {
	registerInput((const NodeOutput **)&m_input, "Input");
}

void manta::ImageOutputNode::registerOutputs() {
	/* void */
}
