#ifndef PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H
#define PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H

#include "float_node_output.h"

namespace piranha {

	class SingleFloatNodeOutput : public FloatNodeOutput {
	public:
		SingleFloatNodeOutput();
		virtual ~SingleFloatNodeOutput();

		virtual void fullOutput(void *target) const;

		double getValue() const { return m_value; }
		void setValue(double v) { m_value = v; }

	protected:
		double m_value;
	};

} /* namespace piranha */

#endif /* PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H */
