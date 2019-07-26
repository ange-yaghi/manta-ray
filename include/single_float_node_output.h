#ifndef PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H
#define PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H

#include "float_node_output.h"

#include "intersection_point.h"

namespace manta {

	class SingleFloatNodeOutput : public FloatNodeOutput {
	public:
		SingleFloatNodeOutput();
		virtual ~SingleFloatNodeOutput();

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		math::real_d getValue() const { return (math::real_d)math::getScalar(m_value); }
		void setValue(const math::real_d &v) { m_value = math::loadScalar((math::real)v); }

	protected:
		math::Vector m_value;
	};

} /* namespace manta */

#endif /* PIRANHA_SINGLE_FLOAT_NODE_OUTPUT_H */
