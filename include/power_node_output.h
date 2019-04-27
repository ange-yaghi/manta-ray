#ifndef POWER_NODE_OUTPUT_H
#define POWER_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class PowerNodeOutput : public VectorNodeOutput {
	public:
		PowerNodeOutput() {
			setDimensions(1);
			setDimensionSize(0, 1);

			m_input = nullptr;
			m_powerNode = nullptr;
			m_defaultPower = (math::real)1.0;
		}

		virtual ~PowerNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		void setInput(const VectorNodeOutput *input) { m_input = input; }
		const VectorNodeOutput *getInput() const { return m_input; }

		void setPowerNode(const VectorNodeOutput *powerNode) { m_powerNode = powerNode; }
		const VectorNodeOutput *getPowerNode() const { return m_powerNode; }

		void setDefaultPower(math::real defaultPower) { m_defaultPower = defaultPower; }
		math::real getDefaultPower() const { return m_defaultPower; }

	protected:
		const VectorNodeOutput *m_input;
		const VectorNodeOutput *m_powerNode;
		math::real m_defaultPower;
	};

} /* namespace manta */

#endif /* POWER_NODE_OUTPUT_H */
