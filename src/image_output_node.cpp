#include <image_output_node.h>

#include <vector_node_output.h>
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

	// Resolve the input data
	const VectorMap2D *map = nullptr;
	m_input->getDataReference((const void **)&map);
	if (map != nullptr) {
		map->fillByteBuffer(&byteBuffer, m_gammaCorrection);
	}
	else {
		// Map needs to be generated
		VectorMap2D generatedMap;
		m_input->fullCompute((void *)&generatedMap);
		generatedMap.fillByteBuffer(&byteBuffer, m_gammaCorrection);
		generatedMap.destroy();
	}

	JpegWriter jpegWriter;
	jpegWriter.setQuality(m_jpegQuality);
	jpegWriter.write(&byteBuffer, m_outputFilename.c_str());

	byteBuffer.free();
}

void manta::ImageOutputNode::_destroy() {
	/* void */
}

void manta::ImageOutputNode::registerInputs() {
	registerInput(&m_input, "Input");
}

void manta::ImageOutputNode::registerOutputs() {
	/* void */
}
