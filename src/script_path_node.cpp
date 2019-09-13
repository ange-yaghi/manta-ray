#include "../include/script_path_node.h"

#include "../include/path.h"

manta::ScriptPathNode::ScriptPathNode() {
    /* void */
}

manta::ScriptPathNode::~ScriptPathNode() {
    /* void */
}

void manta::ScriptPathNode::_initialize() {
    m_output.initialize();
}

void manta::ScriptPathNode::_evaluate() {
    Path unitFile = getIrStructure()->getParentUnit()->getPath();
    Path unitPath;

    unitFile.getAbsolute().getParentPath(&unitPath);

    m_output.setData(unitPath.toString() + "/");
}

void manta::ScriptPathNode::_destroy() {
    /* void */
}

void manta::ScriptPathNode::registerOutputs() {
    setPrimaryOutput("path");
    registerOutput(&m_output, "path");
}

void manta::ScriptPathNode::registerInputs() {
    /* void */
}
