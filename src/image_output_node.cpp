#include "../include/image_output_node.h"

#include "../include/vector_node_output.h"
#include "../include/image_byte_buffer.h"
#include "../include/jpeg_writer.h"
#include "../include/path.h"

manta::ImageOutputNode::ImageOutputNode() {
    m_outputFilename = "";
    m_gammaCorrection = false;
    m_jpegQuality = 95;

    m_input = nullptr;
    m_correctGammaInput = nullptr;
    m_jpegQualityInput = nullptr;
    m_filenameInput = nullptr;
}

manta::ImageOutputNode::~ImageOutputNode() {
    /* void */
}

void manta::ImageOutputNode::_initialize() {
    /* void */
}

void manta::ImageOutputNode::_evaluate() {
    ImageByteBuffer byteBuffer;

    piranha::native_bool gammaCorrection = m_gammaCorrection;
    piranha::native_int jpegQuality = m_jpegQuality;
    piranha::native_string filename = m_outputFilename;

    if (m_correctGammaInput != nullptr) 
        static_cast<piranha::NodeOutput *>(m_correctGammaInput)->fullCompute((void *)&gammaCorrection);
    if (m_jpegQualityInput != nullptr)
        static_cast<piranha::NodeOutput *>(m_jpegQualityInput)->fullCompute((void *)&jpegQuality);
    if (m_filenameInput != nullptr) {
        std::string rawFilename;
        static_cast<piranha::NodeOutput *>(m_filenameInput)->fullCompute((void *)&rawFilename);

        Path finalPath;
        const Path rawFilePath(rawFilename);
        resolvePath(&rawFilePath, &finalPath);

        filename = finalPath.toString();
    }

    // Resolve the input data
    VectorMap2D map;
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
    input->calculateAllDimensions(&map);
    map.fillByteBuffer(&byteBuffer, gammaCorrection);

    JpegWriter jpegWriter;
    jpegWriter.setQuality(jpegQuality);
    jpegWriter.write(&byteBuffer, filename.c_str());

    byteBuffer.free();
    map.destroy();
}

void manta::ImageOutputNode::_destroy() {
    /* void */
}

void manta::ImageOutputNode::registerInputs() {
    registerInput(&m_input, "map");
    registerInput(&m_filenameInput, "filename");
    registerInput(&m_jpegQualityInput, "jpeg_quality");
    registerInput(&m_correctGammaInput, "correct_gamma");
}

void manta::ImageOutputNode::registerOutputs() {
    /* void */
}
