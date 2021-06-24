#include "../include/complex_node_output.h"

#include "../include/complex_split_node.h"

const piranha::ChannelType manta::ComplexNodeOutput::ComplexType("ComplexNodeType");

manta::ComplexNodeOutput::ComplexNodeOutput() : StreamingNodeOutput(&ComplexType) {
    /* void */
}

manta::ComplexNodeOutput::ComplexNodeOutput(const piranha::ChannelType *channelType)
    : StreamingNodeOutput(&ComplexType)
{
    /* void */
}

manta::ComplexNodeOutput::~ComplexNodeOutput() {
    /* void */
}

piranha::Node *manta::ComplexNodeOutput::newInterface(piranha::NodeAllocator *nodeAllocator) {
    ComplexSplitNode *complexInterface = nodeAllocator->allocate<ComplexSplitNode>();
    complexInterface->initialize();
    complexInterface->connectInput(this, "__in", nullptr);

    return complexInterface;
}
