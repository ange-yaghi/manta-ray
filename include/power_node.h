#ifndef POWER_NODE_H
#define POWER_NODE_H

#include <node.h>

#include <vector_node_output.h>
#include <power_node_output.h>
#include <manta_math.h>

namespace manta {

	class PowerNode : public Node {
	public:
		PowerNode();
		PowerNode(math::real power, const VectorNodeOutput *input);
		~PowerNode();

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

		void setInputNode(const VectorNodeOutput *inputNode) { m_inputNode = inputNode; }
		const VectorNodeOutput *getInputNode() const { return m_inputNode; }

		void setPowerNode(const VectorNodeOutput *powerNode) { m_powerNode = powerNode; }
		const VectorNodeOutput *getPowerNode() const { return m_powerNode; }

		const PowerNodeOutput *getMainOutput() const { return &m_output; }

	protected:
		virtual void _initialize();

		virtual void registerInputs();
		virtual void registerOutputs();

		const VectorNodeOutput *m_inputNode;
		const VectorNodeOutput *m_powerNode;
		PowerNodeOutput m_output;

		math::real m_power;
	};

} /* namespace manta */

#endif /* POWER_NODE_H */
