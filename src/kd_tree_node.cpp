#include "../include/kd_tree_node.h"
#include "../include/vector_node_output.h"
#include "../include/session.h"
#include "../include/kd_tree.h"
#include "../include/console.h"

#include "../include/mesh.h"

manta::KdTreeNode::KdTreeNode() {
    m_kdTree = nullptr;

    m_meshInput = nullptr;
    m_granularityInput = nullptr;
    m_widthInput = nullptr;
    m_centerInput = nullptr;
    m_cacheKeyInput = nullptr;
    m_overwriteCacheInput = nullptr;
}

manta::KdTreeNode::~KdTreeNode() {
    /* void */
}

void manta::KdTreeNode::_initialize() {
    /* void */
}

void manta::KdTreeNode::_evaluate() {
    Mesh *mesh = getObject<Mesh>(m_meshInput);

    piranha::native_float width;
    piranha::native_int granularity;
    piranha::native_string cacheKey;
    piranha::native_bool overwriteCache;
    math::Vector center;

    m_widthInput->fullCompute((void *)&width);
    m_granularityInput->fullCompute((void *)&granularity);
    m_cacheKeyInput->fullCompute((void *)&cacheKey);
    m_overwriteCacheInput->fullCompute((void *)&overwriteCache);
    static_cast<VectorNodeOutput *>(m_centerInput)->sample(nullptr, (void *)&center);

    m_kdTree = overwriteCache
        ? nullptr
        : manta::Session::get().getCachedKdTree(cacheKey);

    if (m_kdTree == nullptr) {
        m_kdTree = new KDTree;
        m_kdTree->configure((math::real)width, center);
        m_kdTree->analyzeWithProgress(mesh, granularity);

        manta::Session::get().putCachedKdTree(cacheKey, m_kdTree);
    }
    else {
        manta::Session::get().getConsole()->out("Using cached kd-tree: " + cacheKey + "\n");
        m_kdTree->setMesh(mesh);
    }
    
    m_output.setReference(m_kdTree);

    //m_kdTree->writeToObjFile("kd_tree_debug.obj");
}

void manta::KdTreeNode::_destroy() {
    /* void */
}

void manta::KdTreeNode::registerOutputs() {
    registerOutput(&m_output, "__out");
    setPrimaryOutput("__out");
}

void manta::KdTreeNode::registerInputs() {
    registerInput(&m_centerInput, "center");
    registerInput(&m_meshInput, "mesh");
    registerInput(&m_widthInput, "width");
    registerInput(&m_granularityInput, "granularity");
    registerInput(&m_cacheKeyInput, "cache_key");
    registerInput(&m_overwriteCacheInput, "overwrite_cache");
    registerInput(&m_meshInput, "mesh");
}
