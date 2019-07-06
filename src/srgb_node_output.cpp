#include <srgb_node_output.h>

#include <rgb_space.h>
#include <color.h>


manta::SrgbNodeOutput::SrgbNodeOutput() {
	/* void */
}

manta::SrgbNodeOutput::~SrgbNodeOutput() {
	/* void */
}

void manta::SrgbNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	(void)surfaceInteraction;

	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector r, g, b, a;

	m_r->sample(surfaceInteraction, (void *)&r);
	m_g->sample(surfaceInteraction, (void *)&g);
	m_b->sample(surfaceInteraction, (void *)&b);
	m_a->sample(surfaceInteraction, (void *)&a);

	math::real_d r_d = math::getScalar(r);
	math::real_d g_d = math::getScalar(g);
	math::real_d b_d = math::getScalar(b);
	math::real_d a_d = math::getScalar(a);

	r_d = RgbSpace::srgb.applyGammaSrgb(r_d);
	g_d = RgbSpace::srgb.applyGammaSrgb(g_d);
	b_d = RgbSpace::srgb.applyGammaSrgb(b_d);
	a_d = RgbSpace::srgb.applyGammaSrgb(a_d);

	*target = math::loadVector(
		(math::real)r_d,
		(math::real)g_d,
		(math::real)b_d,
		(math::real)a_d
	);
}

void manta::SrgbNodeOutput::discreteSample2D(int x, int y, void *target) const {
	(void)x;
	(void)y;

	sample(nullptr, target);
}

void manta::SrgbNodeOutput::fullOutput(const void **_target) const {
	// TODO
	*_target = nullptr;
}

void manta::SrgbNodeOutput::registerInputs() {
	registerInput(&m_a);
	registerInput(&m_a);
	registerInput(&m_a);
	registerInput(&m_a);
}
