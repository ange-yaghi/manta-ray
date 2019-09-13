#include "../include/append_path_node.h"

#include "../include/path.h"

manta::AppendPathNode::AppendPathNode() {
    /* void */
}

manta::AppendPathNode::~AppendPathNode() {
    /* void */
}

void manta::AppendPathNode::_initialize() {
    m_output.initialize();
}

void manta::AppendPathNode::_evaluate() {
    std::string basePathStr, pathStr;

    m_basePath->fullCompute((void *)&basePathStr);
    m_path->fullCompute((void *)&pathStr);

    Path basePath(basePathStr), path(pathStr);

    if (path.isAbsolute()) {
        m_output.setData(path.toString());
    }
    else {
        m_output.setData(
            basePath.append(path).toString()
        );
    }
}

void manta::AppendPathNode::_destroy() {
    /* void */
}

void manta::AppendPathNode::registerOutputs() {
    setPrimaryOutput("result");
    registerOutput(&m_output, "result");
}

void manta::AppendPathNode::registerInputs() {
    registerInput(&m_basePath, "base");
    registerInput(&m_path, "path");
}
