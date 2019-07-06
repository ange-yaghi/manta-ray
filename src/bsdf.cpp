#include <bsdf.h>

manta::BSDF::BSDF() {
	/* void */
}

manta::BSDF::~BSDF() {
	/* void */
}

void manta::BSDF::registerOutputs() {
	setPrimaryOutput(&m_output);

	registerOutput(&m_output, "$primary");
}
