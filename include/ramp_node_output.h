#ifndef RAMP_NODE_OUTPUT_H
#define RAMP_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class RampNodeOutput : public VectorNodeOutput {
	public:
		RampNodeOutput() {
			setDimensions(1);
			setDimensionSize(0, 1);

			m_input = nullptr;
			m_foot = nullptr;
			m_slope = nullptr;
			m_dc = nullptr;

			m_defaultFoot = math::constants::Zero;
			m_defaultSlope = math::constants::One;
			m_defaultDc = math::constants::Zero;
		}

		virtual ~RampNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;
		virtual void fullOutput(const void **target) const;

		void setInput(const VectorNodeOutput *input) { m_input = input; }
		const NodeOutput *getInput() const { return m_input; }

		void setFootNode(const VectorNodeOutput *footNode) { m_foot = footNode; }
		const NodeOutput *getFootNode() const { return m_foot; }

		void setSlopeNode(const VectorNodeOutput *slopeNode) { m_slope = slopeNode; }
		const NodeOutput *getSlopeNode() const { return m_slope; }

		void setDcNode(const VectorNodeOutput *dcNode) { m_dc = dcNode; }
		const NodeOutput *getDcNode() const { return m_dc; }

		void setDefaultFoot(const math::Vector &v) { m_defaultFoot = v; }
		math::Vector getDefaultFoot() const { return m_defaultFoot; }

		void setDefaultSlope(const math::Vector &v) { m_defaultSlope = v; }
		math::Vector getDefaultSlopt() const { return m_defaultSlope; }

		void setDefaultDc(const math::Vector &v) { m_defaultDc = v; }
		math::Vector getDefaultDc() const { return m_defaultDc; }

		static math::Vector ramp(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &slope);

		pNodeInput *getInputConnection() { return &m_input; }
		pNodeInput *getFootConnection() { return &m_foot; }
		pNodeInput *getSlopeConnection() { return &m_slope; }
		pNodeInput *getDcConnection() { return &m_dc; }

	protected:
		pNodeInput m_input;
		pNodeInput m_foot;
		pNodeInput m_slope;
		pNodeInput m_dc;
		math::Vector m_defaultFoot;
		math::Vector m_defaultSlope;
		math::Vector m_defaultDc;
	};

} /* namespace manta */

#endif /* RAMP_NODE_OUTPUT_H */
