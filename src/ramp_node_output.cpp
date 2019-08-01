#include <ramp_node_output.h>

void manta::RampNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
	VectorNodeOutput *dc = static_cast<VectorNodeOutput *>(m_dc);
	VectorNodeOutput *foot = static_cast<VectorNodeOutput *>(m_foot);
	VectorNodeOutput *slope = static_cast<VectorNodeOutput *>(m_slope);
	
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector 
		v_input = math::constants::Zero, 
		v_dc = m_defaultDc,
		v_foot = m_defaultFoot,
		v_slope = m_defaultSlope;

	if (input != nullptr) {
		input->sample(surfaceInteraction, (void *)&v_input);
	}

	if (dc != nullptr) {
		dc->sample(surfaceInteraction, (void *)&v_dc);
	}

	if (foot != nullptr) {
		foot->sample(surfaceInteraction, (void *)&v_foot);
	}

	if (slope != nullptr) {
		slope->sample(surfaceInteraction, (void *)&v_slope);
	}

	*target = ramp(v_input, v_dc, v_foot, v_slope);
}

void manta::RampNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
	VectorNodeOutput *dc = static_cast<VectorNodeOutput *>(m_dc);
	VectorNodeOutput *foot = static_cast<VectorNodeOutput *>(m_foot);
	VectorNodeOutput *slope = static_cast<VectorNodeOutput *>(m_slope);

	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector
		v_input = math::constants::Zero,
		v_dc = m_defaultDc,
		v_foot = m_defaultFoot,
		v_slope = m_defaultSlope;

	if (input != nullptr) {
		input->discreteSample2d(x, y, (void *)&v_input);
	}

	if (dc != nullptr) {
		input->discreteSample2d(x, y, (void *)&v_dc);
	}

	if (foot != nullptr) {
		input->discreteSample2d(x, y, (void *)&v_foot);
	}

	if (slope != nullptr) {
		slope->discreteSample2d(x, y, (void *)&v_slope);
	}

	*target = ramp(v_input, v_dc, v_foot, v_slope);
}

manta::math::Vector manta::RampNodeOutput::ramp(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &slope) {
	math::Vector s = math::sub(input, foot);
	s = math::componentMax(s, math::constants::Zero); // Clip
	s = math::mul(s, slope);
	s = math::add(s, dc);

	return s;
}
