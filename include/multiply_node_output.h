#ifndef MULTIPLY_NODE_OUTPUT_H
#define MULTIPLY_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>

namespace manta {

	class MultiplyNodeOutput : public VectorNodeOutput {
	public:
		MultiplyNodeOutput() {
			m_inputA = nullptr;
			m_inputB = nullptr;

			m_defaultA = math::constants::Zero;
			m_defaultB = math::constants::Zero;
		}

		virtual ~MultiplyNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void getDataReference(const void **target) const;

		void setInputA(pNodeInput input) { m_inputA = input; }
		pNodeInput getInputA() const { return m_inputA; }

		void setInputB(pNodeInput input) { m_inputB = input; }
		pNodeInput getInputB() const { return m_inputB; }

		void setDefaultA(const math::Vector &v) { m_defaultA = v; }
		math::Vector getDefaultA() const { return m_defaultA; }

		void setDefaultB(const math::Vector &v) { m_defaultB = v; }
		math::Vector getDefaultB() const { return m_defaultB; }

		pNodeInput *getInputAConnection() { return &m_inputA; }
		pNodeInput *getInputBConnection() { return &m_inputB; }

	protected:
		virtual void _evaluateDimensions();

	protected:
		pNodeInput m_inputA;
		pNodeInput m_inputB;
		math::Vector m_defaultA;
		math::Vector m_defaultB;
	};

} /* namespace manta */

#endif /* MULTIPLY_NODE_OUTPUT_H */
