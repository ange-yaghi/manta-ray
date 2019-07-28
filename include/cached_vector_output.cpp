#include "cached_vector_output.h"

manta::CachedVectorOutput::CachedVectorOutput() {
	m_value = math::constants::Zero;
}

manta::CachedVectorOutput::~CachedVectorOutput() {
	/* void */
}

void manta::CachedVectorOutput::sample(
	const IntersectionPoint *surfaceInteraction, void *_target) const 
{
	(void)surfaceInteraction;

	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	*target = m_value;
}

void manta::CachedVectorOutput::discreteSample2D(int x, int y, void *target) const {
	(void)x;
	(void)y;

	sample(nullptr, target);
}

void manta::CachedVectorOutput::fullOutput(const void **target) const {
	*target = nullptr;
}
