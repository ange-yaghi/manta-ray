#include <remap_node_output.h>

void manta::RemapNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector raw;
	m_input->sample(surfaceInteraction, (void *)&raw);

	*target = remap(raw);
}

void manta::RemapNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector raw;
	m_input->discreteSample2D(x, y, (void *)&raw);

	*target = remap(raw);
}

void manta::RemapNodeOutput::fullOutput(const void **_target) const {
	*_target = nullptr;
}

manta::math::Vector manta::RemapNodeOutput::remap(const math::Vector &s) const {
	math::Vector remapped = math::add(
		math::mul(m_start, math::sub(math::constants::One, s)),
		math::mul(m_end, s));

	return remapped;
}
