#include "../include/streaming_node_output.h"

manta::StreamingNodeOutput::
    StreamingNodeOutput(const piranha::ChannelType *channelType) 
        : piranha::NodeOutput(channelType) 
{
    m_dimensionsEvaluated = false;
    m_dimensionCount = 0;

    for (int i = 0; i < 4; ++i) {
        m_dimensions[i] = INT_MAX;
    }
}

manta::StreamingNodeOutput::~StreamingNodeOutput() {
    /* void */
}

int manta::StreamingNodeOutput::getLargestDimensionSize() const {
    const int dimensionCount = m_dimensionCount;
    int largestSize = 0;
    for (int i = 0; i < dimensionCount; i++) {
        if (getSize(i) > largestSize) {
            largestSize = getSize(i);
        }
    }

    return largestSize;
}

void manta::StreamingNodeOutput::evaluateDimensions() {
    if (m_dimensionsEvaluated) return;

    _evaluateDimensions();

    m_dimensionsEvaluated = true;
}

void manta::StreamingNodeOutput::_evaluate() {
    evaluateDimensions();

    if (m_dimensionCount == 2) {
        if (m_dimensions[0] > 2000 || m_dimensions[1] > 2000) {
            throw;
        }
    }
}

void manta::StreamingNodeOutput::_evaluateDimensions() {
    setDimensions(0);
}
