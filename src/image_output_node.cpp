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

    bool gammaCorrection = m_gammaCorrection;
    int jpegQuality = m_jpegQuality;
    std::string filename = m_outputFilename;

    if (m_correctGammaInput != nullptr) 
        static_cast<piranha::NodeOutput *>(m_correctGammaInput)->fullCompute((void *)&gammaCorrection);
    if (m_jpegQualityInput != nullptr)
        static_cast<piranha::NodeOutput *>(m_jpegQualityInput)->fullCompute((void *)&jpegQuality);
    if (m_filenameInput != nullptr) {
        std::string rawFilename;
        static_cast<piranha::NodeOutput *>(m_filenameInput)->fullCompute((void *)&rawFilename);

        Path finalPath;
        resolvePath(&Path(rawFilename), &finalPath);

        filename = finalPath.toString();
    }

    // Resolve the input data
    const VectorMap2D *map = nullptr;
    VectorMap2DNodeOutput *input = static_cast<VectorMap2DNodeOutput *>(m_input);
    map = input->getMap();
    if (map != nullptr) {
        map->fillByteBuffer(&byteBuffer, gammaCorrection);
    }
    else {
        // Map needs to be generated
        VectorMap2D generatedMap;
        m_input->fullCompute((void *)&generatedMap);
        generatedMap.fillByteBuffer(&byteBuffer, gammaCorrection);
        generatedMap.destroy();
    }

    JpegWriter jpegWriter;
    jpegWriter.setQuality(jpegQuality);
    jpegWriter.write(&byteBuffer, filename.c_str());

    byteBuffer.free();
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
