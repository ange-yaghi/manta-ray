#ifndef MANTARAY_UI_BOUNDING_BOX_H
#define MANTARAY_UI_BOUNDING_BOX_H

#include "delta.h"

namespace mantaray_ui {

    class BoundingBox {
    public:
        BoundingBox();
        BoundingBox(float width, float height);
        ~BoundingBox();

        void SetMin(const ysVector2 &minPoint) { m_min = minPoint; }
        ysVector2 GetMin() const { return m_min; }

        void SetMax(const ysVector2 &maxPoint) { m_max = maxPoint; }
        ysVector2 GetMax() const { return m_max; }

        float Width() const;
        float Height() const;

        float CenterX() const;
        float CenterY() const;
        float Right() const { return m_max.x; }
        float Left() const { return m_min.x; }
        float Top() const { return m_max.y; }
        float Bottom() const { return m_min.y; }

        ysVector2 GetCenter() const;
        BoundingBox MarginOffset(float dx, float dy) const;

        BoundingBox &AlignTop(float top);
        BoundingBox &AlignBottom(float bottom);
        BoundingBox &AlignRight(float right);
        BoundingBox &AlignLeft(float left);
        BoundingBox &AlignCenterX(float hCenter);
        BoundingBox &AlignCenterY(float vCenter);
        BoundingBox &PixelPerfect();

        BoundingBox Add(const BoundingBox &box) const;

    protected:
        ysVector2 m_min;
        ysVector2 m_max;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_BOUNDING_BOX_H */
