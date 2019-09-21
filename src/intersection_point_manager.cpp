#include "../include/intersection_point_manager.h"

manta::IntersectionPointManager::IntersectionPointManager() {
    m_threadId = 0;
    m_currentId = 0;
}

manta::IntersectionPointManager::~IntersectionPointManager() {
    /* void */
}

manta::intersection_id manta::IntersectionPointManager::generateId() {
    return m_currentId++;
}
