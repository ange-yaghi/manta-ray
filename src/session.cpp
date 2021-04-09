#include "../include/session.h"

#include "../include/vector_map_2d.h"
#include "../include/kd_tree.h"

manta::Session::Session() {
    m_console = nullptr;
}

manta::Session::~Session() {
    /* void */
}

manta::Session &manta::Session::get() {
    static Session session;
    return session;
}

std::vector<manta::PreviewNode *> manta::Session::getPreviews() {
    std::lock_guard<std::mutex> lock(m_previewLock);
    return m_previews;
}

manta::KDTree *manta::Session::getCachedKdTree(const std::string &key) {
    auto kdTree = m_kdTreeCache.find(key);
    return kdTree == m_kdTreeCache.end()
        ? nullptr
        : kdTree->second;
}

void manta::Session::putCachedKdTree(const std::string &key, KDTree *tree) {
    KDTree *cached = getCachedKdTree(key);
    if (cached != nullptr) {
        delete cached;
    }

    m_kdTreeCache.emplace(key, tree);
}

void manta::Session::registerPreview(PreviewNode *preview) {
    std::lock_guard<std::mutex> lock(m_previewLock);
    m_previews.push_back(preview);
}

void manta::Session::deregisterPreview(PreviewNode *preview) {
    std::lock_guard<std::mutex> lock(m_previewLock);
    const int n = getPreviewCount();
    if (n == 0) return;

    for (int i = 0; i < n; ++i) {
        if (m_previews[i] == preview) {
            m_previews[i] = m_previews[(size_t)n - 1];
            break;
        }
        else if (i == n - 1) {
            return;
        }
    }

    m_previews.resize((size_t)n - 1);
}
