#ifndef MANTARAY_UI_GRID_H
#define MANTARAY_UI_GRID_H

#include "bounding_box.h"

namespace mantaray_ui {

    class Grid {
    public:
        Grid();
        Grid(const BoundingBox &box, int horizontalCells, int verticalCells, float margin);
        Grid(const BoundingBox &box, int horizontalCells, int verticalCells, float horizontalMargin, float verticalMargin);
        ~Grid();

        void SetBoundingBox(const BoundingBox &box) { m_boundingBox = box; }
        BoundingBox GetBoundingBox() const { return m_boundingBox; }

        void SetVerticalDivisions(int divisions) { m_verticalDivisions = divisions; }
        int GetVerticalDivisions() const { return m_verticalDivisions; }

        void SetHorizontalDivisions(int divisions) { m_horizontalDivisions = divisions; }
        int GetHorizontalDivisions() const { return m_horizontalDivisions; }

        void SetVerticalMargin(float margin) { m_verticalMargin = margin; }
        float GetVerticalMargin() const { return m_verticalMargin; }

        void SetHorizontalMargin(float margin) { m_horizontalMargin = margin; }
        float GetHorizontalMargin() const { return m_horizontalMargin; }

        void GetCell(int x, int y, BoundingBox &target) const;
        BoundingBox GetCell(int x, int y) const;
        BoundingBox GetFullCell(int x, int y) const;
        BoundingBox GetRange(int x0, int x1, int y0, int y1) const;
        BoundingBox GetFullRange(int x0, int x1, int y0, int y1) const;

    protected:
        BoundingBox m_boundingBox;

        int m_verticalDivisions;
        int m_horizontalDivisions;

        float m_verticalMargin;
        float m_horizontalMargin;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_GRID_H */
