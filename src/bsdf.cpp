#include <bsdf.h>

#include <stack_allocator.h>

manta::BSDF::BSDF() {
	/* void */
}

manta::BSDF::~BSDF() {
	/* void */
}

void manta::BSDF::registerOutputs() {
	registerOutput(&m_output, "Output");
}
