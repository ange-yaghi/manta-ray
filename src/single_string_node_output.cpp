#include <single_string_node_output.h>

manta::SingleStringNodeOutput::SingleStringNodeOutput() {
	m_value = "";
}

manta::SingleStringNodeOutput::~SingleStringNodeOutput() {
	/* void */
}

void manta::SingleStringNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	(void)surfaceInteraction;

	std::string *target = static_cast<std::string *>(_target);
	*target = m_value;
}

void manta::SingleStringNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	(void)x;
	(void)y;

	sample(nullptr, _target);
}

void manta::SingleStringNodeOutput::fullCompute(void *_target) const {
	sample(nullptr, _target);
}
