#ifndef STEP_NODE_OUTPUT_H
#define STEP_NODE_OUTPUT_H

#include <vector_node_output.h>

#include <node_type.h>
#include <manta_math.h>
#include <vector_map_2d.h>

namespace manta {

	class StepNodeOutput : public VectorNodeOutput {
	public:
		StepNodeOutput() {
			m_input = nullptr;
			m_foot = nullptr;
			m_step = nullptr;
			m_dc = nullptr;

			m_defaultFoot = math::constants::Zero;
			m_defaultStep = math::constants::One;
			m_defaultDc = math::constants::Zero;
		}

		virtual ~StepNodeOutput() {
			/* void */
		}

		virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
		virtual void discreteSample2D(int x, int y, void *target) const;

		void setInput(pNodeInput input) { m_input = input; }
		const NodeOutput *getInput() const { return m_input; }

		void setFootNode(pNodeInput footNode) { m_foot = footNode; }
		const NodeOutput *getFootNode() const { return m_foot; }

		void setStepNode(pNodeInput stepNode) { m_step = stepNode; }
		const NodeOutput *getStepNode() const { return m_step; }

		void setDcNode(pNodeInput dcNode) { m_dc = dcNode; }
		const NodeOutput *getDcNode() const { return m_dc; }

		void setDefaultFoot(const math::Vector &v) { m_defaultFoot = v; }
		math::Vector getDefaultFoot() const { return m_defaultFoot; }

		void setDefaultStep(const math::Vector &v) { m_defaultStep = v; }
		math::Vector getDefaultStep() const { return m_defaultStep; }

		void setDefaultDc(const math::Vector &v) { m_defaultDc = v; }
		math::Vector getDefaultDc() const { return m_defaultDc; }

		static math::Vector stepFunction(const math::Vector &input, const math::Vector &dc, const math::Vector &foot, const math::Vector &step);

		pNodeInput *getInputConnection() { return &m_input; }
		pNodeInput *getFootConnection() { return &m_foot; }
		pNodeInput *getStepConnection() { return &m_step; }
		pNodeInput *getDcConnection() { return &m_dc; }

	protected:
		pNodeInput m_input;
		pNodeInput m_foot;
		pNodeInput m_step;
		pNodeInput m_dc;
		math::Vector m_defaultFoot;
		math::Vector m_defaultStep;
		math::Vector m_defaultDc;
	};

} /* namespace manta */

#endif /* STEP_NODE_OUTPUT_H */
