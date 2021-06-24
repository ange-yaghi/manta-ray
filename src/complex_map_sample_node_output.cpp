#include "../include/complex_map_sample_node_output.h"

#include "../include/complex_map_sampler.h"
#include "../include/intersection_point.h"

manta::ComplexMapSampleNodeOutput::ComplexMapSampleNodeOutput() {
    m_map = nullptr;
    m_sampler = nullptr;
}

manta::ComplexMapSampleNodeOutput::~ComplexMapSampleNodeOutput() {
    /* void */
}

void manta::ComplexMapSampleNodeOutput::sample(
    const IntersectionPoint *surfaceInteraction,
    void *target_) const
{
    const math::real u_p = math::getX(surfaceInteraction->m_textureCoodinates);
    const math::real v_p = math::getY(surfaceInteraction->m_textureCoodinates);

    ComplexMap2D *map = getObject<ComplexMap2D>(m_map);

    math::Complex *target = reinterpret_cast<math::Complex *>(target_);
    *target = m_sampler->sample(map, u_p, v_p);
}

void manta::ComplexMapSampleNodeOutput::discreteSample2d(int x, int y, void *target_) const {
    ComplexMap2D *map = getObject<ComplexMap2D>(m_map);

    math::Complex *target = reinterpret_cast<math::Complex *>(target_);
    *target = map->sampleDiscrete((math::real_d)x, (math::real_d)y);
}

void manta::ComplexMapSampleNodeOutput::fullOutput(const void **target) const {
    *target = nullptr;
}

void manta::ComplexMapSampleNodeOutput::registerInputs() {
    /* void */
}

void manta::ComplexMapSampleNodeOutput::_evaluateDimensions() {
    ComplexMap2D *map = getObject<ComplexMap2D>(m_map);

    setDimensions(2);
    setDimensionSize(0, map->getWidth());
    setDimensionSize(1, map->getHeight());
}
