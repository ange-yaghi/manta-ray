#include <vector_map_2d_node_output.h>

#include <intersection_point.h>

void manta::VectorMap2DNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);

	math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
	math::real v = math::getY(surfaceInteraction->m_textureCoodinates);

	// Wrap coordinates
	u = (math::real)fmod(u, (math::real)1.0);
	v = (math::real)fmod(v, (math::real)1.0);

	if (u < 0) u = 1 + u;
	if (v < 0) v = 1 + v;

	*target = m_map->sample(u, 1 - v);
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

	setDimensionSize(0, map->getWidth());
	setDimensionSize(1, map->getHeight());
}
