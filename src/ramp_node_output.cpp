#include <ramp_node_output.h>

void manta::RampNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector 
		input = math::constants::Zero, 
		dc = m_defaultDc, 
		foot = m_defaultFoot, 
		slope = m_defaultSlope;

	if (m_input != nullptr) {
		m_input->sample(surfaceInteraction, (void *)&input);
	}

	if (m_dc != nullptr) {
		m_dc->sample(surfaceInteraction, (void *)&dc);
	}

	if (m_foot != nullptr) {
		m_foot->sample(surfaceInteraction, (void *)&foot);
	}

	if (m_slope != nullptr) {
		m_slope->sample(surfaceInteraction, (void *)&slope);
	}

	*target = ramp(input, dc, foot, slope);
}

void manta::RampNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector
		input = math::constants::Zero,
		dc = m_defaultDc,
		foot = m_defaultFoot,
		slope = m_defaultSlope;

	if (m_input != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&input);
	}

	if (m_dc != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&dc);
	}

	if (m_foot != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&foot);
	}

	if (m_slope != nullptr) {
		m_slope->discreteSample2D(x, y, (void *)&slope);
	}

	*target = ramp(input, dc, foot, slope);
}

manta::math::Vector manta::RampNodeOutput::ramp(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &slope) {
	math::Vector s = math::sub(input, foot);
	s = math::componentMax(s, math::constants::Zero); // Clip
	s = math::mul(s, slope);
	s = math::add(s, dc);

	return s;
}
