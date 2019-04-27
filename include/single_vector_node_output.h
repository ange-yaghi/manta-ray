#ifndef SINGLE_VECTOR_NODE_OUTPUT_H
#define SINGLE_VECTOR_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class SingleVectorNodeOutput : public VectorNodeOutput {
	public:
		SingleVectorNodeOutput() {
			setDimensions(1);
			setDimensionSize(0, 1);
		}

		virtual ~SingleVectorNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		math::Vector getValue() const { return m_value; }
		void setValue(const math::Vector &v) { m_value = v; }

	protected:
		math::Vector m_value;
	};

} /* namespace manta */

#endif /* SINGLE_VECTOR_NODE_OUTPUT_H */
