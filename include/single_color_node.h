#ifndef SINGLE_COLOR_NODE_H
#define SINGLE_COLOR_NODE_H

#include <node.h>

#include <single_vector_node_output.h>
#include <manta_math.h>

namespace manta {

	class SingleColorNode : public Node {
	public:
		SingleColorNode();
		SingleColorNode(const math::Vector &color);
		~SingleColorNode();

		void setColor(const math::Vector &color) { m_output.setValue(color); }
		math::Vector getColor() const { return m_output.getValue(); }

		const SingleVectorNodeOutput *getMainOutput() const { return &m_output; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		SingleVectorNodeOutput m_output;
	};

} /* namespace manta */

#endif /* SINGLE_COLOR_NODE_H */
