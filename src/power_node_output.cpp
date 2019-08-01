#include "../include/power_node_output.h"

void manta::PowerNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
	VectorNodeOutput *powerInput = static_cast<VectorNodeOutput *>(m_powerNode);
	
	math::Vector base = math::constants::Zero;
	math::real power;
	if (input != nullptr) {
		input->sample(surfaceInteraction, (void *)&base);
	}

	if (powerInput != nullptr) {
		math::Vector s;
		powerInput->sample(surfaceInteraction, (void *)&s);
		power = math::getScalar(s);
	}
	else {
		power = m_defaultPower;
	}

	math::real x_pow = ::pow(math::getX(base), power);
	math::real y_pow = ::pow(math::getY(base), power);
	math::real z_pow = ::pow(math::getZ(base), power);

	math::Vector *target = (math::Vector *)_target;
	*target = math::loadVector(x_pow, y_pow, z_pow);
}

void manta::PowerNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
	VectorNodeOutput *powerInput = static_cast<VectorNodeOutput *>(m_powerNode);

	math::Vector base = math::constants::Zero;
	math::real power;
	if (input != nullptr) {
		input->discreteSample2d(x, y, (void *)&base);
	}

	if (powerInput != nullptr) {
		math::Vector s;
		powerInput->discreteSample2d(x, y, (void *)&s);
		power = math::getScalar(s);
	}
	else {
		power = m_defaultPower;
	}

	math::real x_pow = ::pow(math::getX(base), power);
	math::real y_pow = ::pow(math::getY(base), power);
	math::real z_pow = ::pow(math::getZ(base), power);

	math::Vector *target = (math::Vector *)_target;
	*target = math::loadVector(x_pow, y_pow, z_pow);
}

void manta::PowerNodeOutput::fullOutput(const void **target) const {
	/* void */
}
