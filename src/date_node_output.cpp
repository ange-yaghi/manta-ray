#include "../include/date_node_output.h"

#include "../include/date_interface_node.h"
#include "../include/standard_allocator.h"

const piranha::ChannelType manta::DateNodeOutput::DateType("DateType");

manta::DateNodeOutput::DateNodeOutput() : piranha::NodeOutput(&DateType) {
    /* void */
}

manta::DateNodeOutput::~DateNodeOutput() {
    /* void */
}

void manta::DateNodeOutput::fullCompute(void *_target) const {
    time_t *target = reinterpret_cast<time_t *>(_target);
    *target = m_time;
}

piranha::Node *manta::DateNodeOutput::newInterface(piranha::NodeAllocator *nodeAllocator) {
    DateInterfaceNode *dateInterface =
        nodeAllocator->allocate<DateInterfaceNode>();
    dateInterface->initialize();
    dateInterface->connectInput(this, "__in", nullptr);

    return dateInterface;
}
