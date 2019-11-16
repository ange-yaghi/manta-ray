#include "../include/string_conversions.h"

#include "../include/vector_node_output.h"

manta::VectorToStringConversionOutput::VectorToStringConversionOutput() {
    m_input = nullptr;
}

manta::VectorToStringConversionOutput::~VectorToStringConversionOutput() {
    /* void */
}

void manta::VectorToStringConversionOutput::fullCompute(void *target) const {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);

    std::string output;

    int dimensionCount = input->getDimensions();
    if (dimensionCount > 0) {
        std::stringstream ss;
        ss << "[" << dimensionCount << "D map ";
        for (int dim = 0; dim < dimensionCount; dim++) {
            int dimensionSize = input->getSize(dim);
            ss << dimensionSize;
            if (dim != dimensionCount - 1) ss << "x";
        }

        ss << "]";
        output = ss.str();
    }
    else {
        math::Vector value;
        input->sample(nullptr, (void *)&value);

        std::stringstream ss;
        ss << "[" <<
            math::getX(value) << ", " <<
            math::getY(value) << ", " <<
            math::getZ(value) << ", " <<
            math::getW(value) << "]";

        output = ss.str();
    }

    std::string *outputAddress = reinterpret_cast<std::string *>(target);
    *outputAddress = output;
}

void manta::VectorToStringConversionOutput::registerInputs() {
    registerInput(&m_input);
}
