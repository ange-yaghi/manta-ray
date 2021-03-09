#include "../include/session.h"

#include "../include/vector_map_2d.h"

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

manta::ImagePreview *manta::Session::createImagePreview(const std::string &name) {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);

    ImagePreview *newPreview = new ImagePreview;
    newPreview->name = name;
    newPreview->map = nullptr;

    m_imagePreviews.push_back(newPreview);

    return newPreview;
}

void manta::Session::destroyImagePreview(ImagePreview *preview) {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);

    if (preview->map != nullptr) {
        preview->map->destroy();
        delete preview->map;
    }
    
    const int count = m_imagePreviews.size();
    for (int i = 0; i < count; ++i) {
        if (m_imagePreviews[i] == preview) {
            m_imagePreviews[i] = m_imagePreviews[count - 1];
        }
    }

    m_imagePreviews.resize(count - 1);

    delete preview; 
}

void manta::Session::attachTargetToImagePreview(ImagePreview *preview, VectorMap2D *target) {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);
    preview->map = target;
}

void manta::Session::detachTargetFromImagePreview(ImagePreview *preview) {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);
    preview->map = nullptr;
}

void manta::Session::clearImagePreviews() {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);

    for (ImagePreview *preview : m_imagePreviews) {
        if (preview->map != nullptr) {
            preview->map->destroy();
            delete preview->map;
        }
        delete preview;
    }
}

void manta::Session::getImagePreviews(std::vector<ImagePreviewContainer> &target) {
    const std::lock_guard<std::mutex> lock(m_imagePreviewLock);

    for (ImagePreview *preview : m_imagePreviews) {
        target.push_back({preview->name, preview->map, preview->finalized});
    }
}
