#include <power_node_output.h>

void manta::PowerNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector base = math::constants::Zero;
	math::real power;
	if (m_input != nullptr) {
		m_input->sample(surfaceInteraction, (void *)&base);
	}

	if (m_powerNode != nullptr) {
		math::Vector s;
		m_input->sample(surfaceInteraction, (void *)&s);
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
	math::Vector base = math::constants::Zero;
	math::real power;
	if (m_input != nullptr) {
		m_input->discreteSample2D(x, y, (void *)&base);
	}

	if (m_powerNode != nullptr) {
		math::Vector s;
		m_input->discreteSample2D(x, y, (void *)&s);
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
