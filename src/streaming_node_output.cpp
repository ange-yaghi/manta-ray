#include "../include/streaming_node_output.h"

manta::StreamingNodeOutput::
    StreamingNodeOutput(const piranha::ChannelType *channelType) 
        : piranha::NodeOutput(channelType) 
{
    m_dimensionsEvaluated = false;
    m_dimensionCount = 0;
}

manta::StreamingNodeOutput::~StreamingNodeOutput() {
    /* void */
}

void manta::StreamingNodeOutput::evaluateDimensions() {
    if (m_dimensionsEvaluated) return;

    _evaluateDimensions();

    m_dimensionsEvaluated = true;
}

void manta::StreamingNodeOutput::_evaluateDimensions() {
    setDimensions(1);
    setDimensionSize(0, 1);
}
