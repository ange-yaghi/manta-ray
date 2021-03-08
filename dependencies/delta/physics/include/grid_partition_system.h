#ifndef DELTA_BASIC_GRID_PARTITION_SYSTEM_H
#define DELTA_BASIC_GRID_PARTITION_SYSTEM_H

#include "delta_core.h"

namespace dphysics {

    class RigidBody;
    class GridPartitionSystem;
    class RigidBodySystem;

    class GridCell {
        friend GridPartitionSystem;
        friend RigidBodySystem;

    public:
        GridCell();
        ~GridCell();

        void IncrementRequestCount();
        void DecrementRequestCount();
        int GetRequestCount() const { return m_requestCount; }

    protected:
        int m_x;
        int m_y;
        int m_requestCount;

        bool m_forceProcess;
        bool m_processed;
        bool m_valid;
        bool m_active;

        ysExpandingArray<RigidBody *, 4> m_objects;
    };

    class GridPartitionSystem : public ysObject {
        friend RigidBodySystem;

    public:
        GridPartitionSystem();
        ~GridPartitionSystem();

        void Reset();
        GridCell *GetCell(int x, int y);
        void ProcessRigidBody(RigidBody *object);

        void SetGridCellSize(float gridCellSize) { m_gridCellSize = gridCellSize; if (m_gridCellSize < m_maxObjectSize) m_gridCellSize = m_maxObjectSize; }
        float GetGridCellSize() const { return m_gridCellSize; }

        void AddObject(int x, int y, RigidBody *body);

    protected:
        unsigned __int64 SzudzikHash(int x, int y);
        int CalculateLoad();

        std::map<unsigned __int64, GridCell *> m_gridCells;

        float m_gridCellSize;
        float m_maxObjectSize;
        int m_maxCells;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_GRID_PARTITION_SYSTEM_H */
