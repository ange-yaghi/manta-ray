#include "../include/preview.h"

#include "../../include/rgb_space.h"
#include "../../include/console.h"

mantaray_ui::Preview::Preview() {
    m_engine = nullptr;
    m_updateIndex = 0;
    m_zoom = 0;
    m_previewNode = nullptr;
    m_pan = manta::math::constants::Zero;
    m_updateQueue = 0;
}

mantaray_ui::Preview::~Preview() {
    /* void */
}

void mantaray_ui::Preview::initialize(dbasic::DeltaEngine *engine, int index) {
    m_engine = engine;
    m_index = index;
}

void mantaray_ui::Preview::destroy() {
    std::lock_guard<std::mutex> lock(m_textureLock);

    for (PreviewSnapshot &snapshot : m_previewSnapshots) {
        if (snapshot.texture != nullptr) {
            m_engine->GetDevice()->DestroyTexture(snapshot.texture);
            snapshot.texture = nullptr;
        }
    }
}

ysTexture *mantaray_ui::Preview::getCurrentPreview() {
    std::lock_guard<std::mutex> lock(m_textureLock);
    if (m_previewSnapshots.size() > 0) {
        return m_previewSnapshots.back().texture;
    }
    else {
        return nullptr;
    }
}

ysTexture *mantaray_ui::Preview::createTexture(const manta::VectorMap2D *vectorMap) {
    const int width = vectorMap->getWidth();
    const int height = vectorMap->getHeight();
    unsigned char *buffer = new unsigned char[width * height * 4];

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            const manta::math::Vector v = vectorMap->get(i, j);
            buffer[(j * width + i) * 4 + 0] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getX(v)) * 255), 255));
            buffer[(j * width + i) * 4 + 1] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getY(v)) * 255), 255));
            buffer[(j * width + i) * 4 + 2] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getZ(v)) * 255), 255));
            buffer[(j * width + i) * 4 + 3] = 0xFF;
        }
    }

    ysTexture *newTexture = nullptr;
    ysError error = m_engine->GetDevice()->CreateTexture(&newTexture, width, height, buffer);

    delete[] buffer;

    return newTexture;
}

void mantaray_ui::Preview::update(bool force) {
    if (m_updateQueue > 0 && !force) return;

    ++m_updateQueue;

    m_title = m_previewNode->getTitle();

    std::thread updateThread(&Preview::updateThread, this, m_updateIndex++);
    updateThread.detach();
}

void mantaray_ui::Preview::clean() {
    std::lock_guard<std::mutex> lock(m_textureLock);

    for (PreviewSnapshot &snapshot : m_previewSnapshots) {
        if (snapshot.outdated && snapshot.texture != nullptr) {
            m_engine->GetDevice()->DestroyTexture(snapshot.texture);
            snapshot.texture = nullptr;
        }
    }
}

void mantaray_ui::Preview::updateThread(int index) {
    if (!m_previewNode->isEvaluated()) {
        --m_updateQueue;
        return;
    }

    manta::VectorMap2D *map = new manta::VectorMap2D;
    m_previewNode->getOutput()->calculateAllDimensions(map);

    ysTexture *newTexture = createTexture(map);

    std::lock_guard<std::mutex> lock(m_textureLock);
    if (m_previewSnapshots.size() > 0) {
        if (m_previewSnapshots.back().index > index) {
            m_engine->GetDevice()->DestroyTexture(newTexture);
            map->destroy();
            delete map;

            --m_updateQueue;

            return;
        }
    }

    for (PreviewSnapshot &snapshot : m_previewSnapshots) {
        snapshot.outdated = true;
    }

    m_previewSnapshots.push_back({ newTexture, index, false });

    map->destroy();
    delete map;

    --m_updateQueue;
}
