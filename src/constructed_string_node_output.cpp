#include <constructed_string_node_output.h>

manta::ConstructedStringNodeOutput::ConstructedStringNodeOutput() {
	/* void */
}

manta::ConstructedStringNodeOutput::~ConstructedStringNodeOutput() {
	/* void */
}

void manta::ConstructedStringNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	(void)surfaceInteraction;

	std::string *target = reinterpret_cast<std::string *>(_target);
	std::string value;

	m_stringInput->sample(surfaceInteraction, (void *)&value);

	*target = value;
}

void manta::ConstructedStringNodeOutput::discreteSample2D(int x, int y, void *target) const {
	(void)x;
	(void)y;

	sample(nullptr, target);
}

void manta::ConstructedStringNodeOutput::fullCompute(void *target) const {
	sample(nullptr, target);
}

void manta::ConstructedStringNodeOutput::registerInputs() {
	registerInput(&m_stringInput);
}
