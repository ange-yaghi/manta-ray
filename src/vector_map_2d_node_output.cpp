#include <vector_map_2d_node_output.h>

#include <intersection_point.h>

void manta::VectorMap2DNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
	math::real v = 1 - math::getY(surfaceInteraction->m_textureCoodinates);

	// Wrap coordinates
	int i_u = u * m_map->getWidth();
	int i_v = v * m_map->getHeight();

	i_u %= m_map->getWidth();
	i_v %= m_map->getHeight();

	if (i_u < 0) i_u = m_map->getWidth() + i_u;
	if (i_v < 0) i_v = m_map->getHeight() + i_v;

	*target = m_map->get(i_u, i_v);
}

void manta::VectorMap2DNodeOutput::discreteSample2D(int x, int y, void *_target) const {
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
