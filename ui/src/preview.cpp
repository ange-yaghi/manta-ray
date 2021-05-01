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
    m_texture = nullptr;
    m_swapBuffers = false;
    m_refreshRate = 0.5f;
    m_buffer = nullptr;
    m_width = 0;
    m_height = 0;
}

mantaray_ui::Preview::~Preview() {
    /* void */
}

void mantaray_ui::Preview::initialize(dbasic::DeltaEngine *engine, int index) {
    m_engine = engine;
    m_index = index;
}

void mantaray_ui::Preview::destroy() {
    if (m_texture != nullptr) {
        m_engine->GetDevice()->DestroyTexture(m_texture);
        m_texture = nullptr;
    }

    if (m_buffer != nullptr) {
        delete[] m_buffer;
    }
}

ysTexture *mantaray_ui::Preview::getCurrentPreview() {
    return m_texture;
}

void mantaray_ui::Preview::updateTexture() {
    std::lock_guard<std::mutex> lock(m_bufferLock);

    if (!m_swapBuffers) {
        return;
    }
    else {
        m_swapBuffers = false;
        if (m_texture == nullptr) {
            m_engine->GetDevice()->CreateTexture(&m_texture, m_width, m_height, m_buffer);
        }
        else{
            m_engine->GetDevice()->UpdateTexture(m_texture, m_buffer);
        }
    }
}

void mantaray_ui::Preview::updateBuffer(const manta::VectorMap2D *vectorMap) {
    m_width = vectorMap->getWidth();
    m_height = vectorMap->getHeight();

    unsigned char *buffer = new unsigned char[(size_t)m_width * m_height * 4];

    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            const manta::math::Vector v = vectorMap->get(i, j);
            buffer[(j * m_width + i) * 4 + 0] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getX(v)) * 255), 255));
            buffer[(j * m_width + i) * 4 + 1] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getY(v)) * 255), 255));
            buffer[(j * m_width + i) * 4 + 2] = (int)(min(std::round(manta::RgbSpace::applyGammaSrgb(manta::math::getZ(v)) * 255), 255));
            buffer[(j * m_width + i) * 4 + 3] = 0xFF;
        }
    }

    if (m_buffer == nullptr) {
        m_buffer = new unsigned char[(size_t)m_width * m_height * 4];
    }

    std::lock_guard<std::mutex> lock(m_bufferLock);
    memcpy(m_buffer, buffer, sizeof(unsigned char) * m_width * m_height * 4);

    delete[] buffer;
    m_swapBuffers = true;
}

void mantaray_ui::Preview::update(bool force) {
    if (m_updateQueue > 0 && !force) return;

    ++m_updateQueue;

    m_title = m_previewNode->getTitle();

    std::thread updateThread(&Preview::updateThread, this, m_updateIndex++);
    updateThread.detach();
}

void mantaray_ui::Preview::clean() {
    /* void */
}

void mantaray_ui::Preview::updateThread(int index) {
    if (!m_previewNode->isEvaluated()) {
        --m_updateQueue;
        return;
    }

    manta::VectorMap2D *map = new manta::VectorMap2D;
    m_previewNode->getOutput()->calculateAllDimensions(map);

    updateBuffer(map);

    map->destroy();
    delete map;

    --m_updateQueue;
}
