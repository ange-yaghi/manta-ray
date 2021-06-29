#include "../include/vector_map_2d_node_output.h"

#include "../include/intersection_point.h"

const piranha::ChannelType manta::VectorMap2DNodeOutput::VectorMap2dType("VectorMap2dType", &VectorNodeOutput::VectorType);

manta::VectorMap2DNodeOutput::VectorMap2DNodeOutput() : VectorNodeOutput(&VectorMap2dType) {
    m_map = nullptr;
}

manta::VectorMap2DNodeOutput::~VectorMap2DNodeOutput() {
    /* void */
}

void manta::VectorMap2DNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);

    const math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
    const math::real v = 1 - math::getY(surfaceInteraction->m_textureCoodinates);

    *target = m_map->triangleSample(u, v);
}

void manta::VectorMap2DNodeOutput::discreteSample2d(int x, int y, void *_target) const {
    math::Vector *target = reinterpret_cast<math::Vector *>(_target);

    *target = m_map->get(x, y);
}

void manta::VectorMap2DNodeOutput::fullOutput(const void **_target) const {
    const VectorMap2D **target = reinterpret_cast<const VectorMap2D **>(_target);

    *target = m_map;
}

void manta::VectorMap2DNodeOutput::setMap(const VectorMap2D *map) {
    m_map = map;
}

void manta::VectorMap2DNodeOutput::_evaluateDimensions() {
    setDimensions(2); 
    setDimensionSize(0, m_map->getWidth()); 
    setDimensionSize(1, m_map->getHeight());
}
