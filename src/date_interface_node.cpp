#include "../include/date_interface_node.h"

#include <iostream>
#include <time.h>

manta::DateInterfaceNode::DateInterfaceNode() {
    m_dateInput = nullptr;
}

manta::DateInterfaceNode::~DateInterfaceNode() {
    /* void */
}

void manta::DateInterfaceNode::_initialize() {
    m_asStringOutput.initialize();
}

void manta::DateInterfaceNode::_evaluate() {
    time_t time;
    m_dateInput->fullCompute((void *)&time);

    struct tm timeInfo;
    localtime_s(&timeInfo, &time);

    char buffer[256];
    strftime(buffer, 256, "%F_T%H_%M_%S", &timeInfo);
    
    m_asStringOutput.setData(buffer);
}

void manta::DateInterfaceNode::_destroy() {
    /* void */
}

void manta::DateInterfaceNode::registerInputs() {
    setInterfaceInput(&m_dateInput);
    registerInput(&m_dateInput, "__in");
}

void manta::DateInterfaceNode::registerOutputs() {
    registerOutput(&m_asStringOutput, "as_string");
}
