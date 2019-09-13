#include "../include/main_script_path.h"

#include "../include/path.h"

manta::MainScriptPathNode::MainScriptPathNode() {
    /* void */
}

manta::MainScriptPathNode::~MainScriptPathNode() {
    /* void */
}

void manta::MainScriptPathNode::_initialize() {
    m_output.initialize();
}

void manta::MainScriptPathNode::_evaluate() {
    Path unitFile = getProgram()->getRootUnit()->getPath();
    Path unitPath;

    unitFile.getAbsolute().getParentPath(&unitPath);

    m_output.setData(unitPath.toString() + "/");
}

void manta::MainScriptPathNode::_destroy() {
    /* void */
}

void manta::MainScriptPathNode::registerOutputs() {
    setPrimaryOutput("path");
    registerOutput(&m_output, "path");
}

void manta::MainScriptPathNode::registerInputs() {
    /* void */
}
