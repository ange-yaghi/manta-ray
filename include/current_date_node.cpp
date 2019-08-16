#include "current_date_node.h"

manta::CurrentDateNode::CurrentDateNode() {
    /* void */
}

manta::CurrentDateNode::~CurrentDateNode() {
    /* void */
}

void manta::CurrentDateNode::_initialize() {
    m_dateOutput.initialize();
}

void manta::CurrentDateNode::_evaluate() {
    time_t currentTime;

    time(&currentTime);
    m_dateOutput.setTime(currentTime);
}

void manta::CurrentDateNode::_destroy() {
    /* void */
}

void manta::CurrentDateNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_dateOutput, "__out");
}
