#include "../include/bounding_box.h"

mantaray_ui::BoundingBox::BoundingBox() {
    m_min = { FLT_MAX, FLT_MAX };
    m_max = { -FLT_MAX, -FLT_MAX };
}

mantaray_ui::BoundingBox::BoundingBox(float width, float height) {
    m_min = { 0.0f, 0.0f };
    m_max = { width, height };
}

mantaray_ui::BoundingBox::~BoundingBox() {
    /* void */
}

float mantaray_ui::BoundingBox::Width() const {
    return m_max.x - m_min.x;
}

float mantaray_ui::BoundingBox::Height() const {
    return m_max.y - m_min.y;
}

float mantaray_ui::BoundingBox::CenterX() const {
    return (m_min.x + m_max.x) / 2.0f;
}

float mantaray_ui::BoundingBox::CenterY() const {
    return (m_min.y + m_max.y) / 2.0f;
}

ysVector2 mantaray_ui::BoundingBox::GetCenter() const {
    return { CenterX(), CenterY() };
}

mantaray_ui::BoundingBox mantaray_ui::BoundingBox::MarginOffset(float dx, float dy) const {
    return BoundingBox(Width() + dx * 2, Height() + dy * 2)
        .AlignCenterX(CenterX())
        .AlignCenterY(CenterY());
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignTop(float top) {
    const float dy = Top() - top;
    m_min.y -= dy;
    m_max.y -= dy;

    return *this;
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignBottom(float bottom) {
    const float dy = Bottom() - bottom;
    m_min.y -= dy;
    m_max.y -= dy;

    return *this;
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignRight(float right) {
    const float dx = Right() - right;
    m_min.x -= dx;
    m_max.x -= dx;

    return *this;
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignLeft(float left) {
    const float dx = Left() - left;
    m_min.x -= dx;
    m_max.x -= dx;

    return *this;
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignCenterX(float hCenter) {
    const float dx = CenterX() - hCenter;
    m_min.x -= dx;
    m_max.x -= dx;

    return *this;
}

mantaray_ui::BoundingBox &mantaray_ui::BoundingBox::AlignCenterY(float vCenter) {
    const float dy = CenterY() - vCenter;
    m_min.y -= dy;
    m_max.y -= dy;

    return *this;
}

mantaray_ui::BoundingBox mantaray_ui::BoundingBox::Add(const BoundingBox &box) const {
    BoundingBox result;
    result.m_min.x = min(box.m_min.x, m_min.x);
    result.m_max.x = max(box.m_max.x, m_max.x);
    result.m_min.y = min(box.m_min.y, m_min.y);
    result.m_max.y = max(box.m_max.y, m_max.y);

    return result;
}
