#ifndef MANTARAY_INTERSECTION_POINT_MANAGER_H
#define MANTARAY_INTERSECTION_POINT_MANAGER_H

#include "intersection_point_types.h"

namespace manta {

    class IntersectionPointManager {
    public:
        IntersectionPointManager();
        ~IntersectionPointManager();

        intersection_id generateId();

        int getThreadId() const { return m_threadId; }
        void setThreadId(int threadId) { m_threadId = threadId; }

    protected:
        int m_threadId;
        intersection_id m_currentId;
    };

} /* namespace manta */

#endif /* MANTARAY_INTERSECTION_POINT_MANAGER_H */
