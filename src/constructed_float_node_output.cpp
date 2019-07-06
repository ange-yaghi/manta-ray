#include <constructed_float_node_output.h>

#include <float_node_output.h>
#include <string_node_output.h>

#include <sstream>

manta::ConstructedFloatNodeOutput::ConstructedFloatNodeOutput() {
	m_input = nullptr;
}

manta::ConstructedFloatNodeOutput::~ConstructedFloatNodeOutput() {
	/* void */
}

void manta::ConstructedFloatNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target) const {
	if (m_input->isType(FloatNodeOutput::FloatType)) {
		m_input->sample(surfaceInteraction, target);
	}
	else if (m_input->isType(StringNodeOutput::StringType)) {
		std::string str;
		m_input->sample(surfaceInteraction, (void *)&str);

		std::stringstream ss(str);
		math::real v;
		ss >> v;

		math::Vector *vecTarget = (math::Vector *)target;
		*vecTarget = math::loadScalar(v);
	}
}

void manta::ConstructedFloatNodeOutput::discreteSample2D(int x, int y, void *target) const {
	if (m_input->isType(FloatNodeOutput::FloatType)) {
		m_input->discreteSample2D(x, y, target);
	}
	else if (m_input->isType(StringNodeOutput::StringType)) {
		std::string str;
		m_input->discreteSample2D(x, y, (void *)&str);

		std::stringstream ss(str);
		math::real v;
		ss >> v;

		math::Vector *vecTarget = (math::Vector *)target;
		*vecTarget = math::loadScalar(v);
	}
}

void manta::ConstructedFloatNodeOutput::fullCompute(void *target) const {
	discreteSample2D(0, 0, target);
}

void manta::ConstructedFloatNodeOutput::registerInputs() {
	registerInput(&m_input);
}
