#ifndef MANTARAY_SINGLE_COLOR_NODE_H
#define MANTARAY_SINGLE_COLOR_NODE_H

#include <piranha.h>

#include "single_vector_node_output.h"
#include "manta_math.h"

namespace manta {

	class SingleColorNode : public piranha::Node {
	public:
		SingleColorNode();
		SingleColorNode(const math::Vector &color);
		virtual ~SingleColorNode();

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

#endif /* MANTARAY_SINGLE_COLOR_NODE_H */
