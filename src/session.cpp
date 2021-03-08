#include "../include/session.h"

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

manta::ImagePlanePreview *manta::Session::createImagePlanePreview(const std::string &name) {
    ImagePlanePreview *newPreview = new ImagePlanePreview;
    newPreview->name = name;
    newPreview->map = nullptr;

    m_imagePlanePreviews.push_back(newPreview);

    return newPreview;
}
