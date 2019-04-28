#include <multiply_node_output.h>

#include <assert.h>

void manta::MultiplyNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector a, b;
	a = m_defaultA;
	b = m_defaultB;

	if (m_inputA != nullptr) {
		m_inputA->sample(surfaceInteraction, (void *)&a);
	}
	if (m_inputB != nullptr) {
		m_inputB->sample(surfaceInteraction, (void *)&b);
	}

	*target = math::mul(a, b);
}

void manta::MultiplyNodeOutput::discreteSample2D(int x, int y, void *_target) const {
	math::Vector *target = reinterpret_cast<math::Vector *>(_target);
	math::Vector a, b;
	a = m_defaultA;
	b = m_defaultB;

	if (m_inputA != nullptr) {
		m_inputA->discreteSample2D(x, y, (void *)&a);
	}
	if (m_inputB != nullptr) {
		m_inputB->discreteSample2D(x, y, (void *)&b);
	}

	*target = math::mul(a, b);
}

void manta::MultiplyNodeOutput::getDataReference(const void **_target) const {
	*_target = nullptr;
}

void manta::MultiplyNodeOutput::_evaluateDimensions() {
	if (m_inputA == nullptr && m_inputB == nullptr) {
		setDimensions(1);
		setDimensionSize(0, 1);
		return;
	}

	if (m_inputA != nullptr) assert(m_inputA->areDimensionsEvaluated());
	if (m_inputB != nullptr) assert(m_inputB->areDimensionsEvaluated());

	int dimensions = 0;
	if (m_inputA != nullptr && m_inputA->getDimensions() > dimensions) dimensions = m_inputA->getDimensions();
	if (m_inputB != nullptr && m_inputB->getDimensions() > dimensions) dimensions = m_inputB->getDimensions();

	setDimensions(dimensions);

	for (int i = 0; i < dimensions; i++) {
		int size = 0;
		if (m_inputA != nullptr && m_inputA->getSize(i) > size) size = m_inputA->getSize(i);
		if (m_inputB != nullptr && m_inputB->getSize(i) > size) size = m_inputB->getSize(i);

		setDimensionSize(i, size);
	}
}
