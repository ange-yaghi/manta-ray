#include <step_node_output.h>

void manta::StepNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector
		input = math::constants::Zero,
		dc = m_defaultDc,
		foot = m_defaultFoot,
		step = m_defaultStep;

	if (m_input != nullptr) {
		m_input->sample(surfaceInteraction, (void *)&input);
	}

	if (m_dc != nullptr) {
		m_dc->sample(surfaceInteraction, (void *)&dc);
	}

	if (m_foot != nullptr) {
		m_foot->sample(surfaceInteraction, (void *)&foot);
	}

	if (m_step != nullptr) {
		m_step->sample(surfaceInteraction, (void *)&step);
	}

	*target = stepFunction(input, dc, foot, step);
}

void manta::StepNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector
		input = math::constants::Zero,
		dc = m_defaultDc,
		foot = m_defaultFoot,
		step = m_defaultStep;

	if (m_input != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&input);
	}

	if (m_dc != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&dc);
	}

	if (m_foot != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&foot);
	}

	if (m_step != nullptr) {
		m_step->discreteSample2D(x, y, (void *)&step);
	}

	*target = stepFunction(input, dc, foot, step);
}

manta::math::Vector manta::StepNodeOutput::stepFunction(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &step) {
	math::Vector s = math::sub(input, foot);
	s = math::gt(s, math::constants::Zero);

	s = math::mul(s, math::sub(step, dc));
	s = math::add(s, dc);

	return s;
}
