#ifndef PIRANHA_STEP_NODE_OUTPUT_H
#define PIRANHA_STEP_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "manta_math.h"
#include "vector_map_2d.h"
#include "intersection_point.h"

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

        void setInput(piranha::pNodeInput input) { m_input = input; }
        const NodeOutput *getInput() const { return m_input; }

        void setFootNode(piranha::pNodeInput footNode) { m_foot = footNode; }
        const NodeOutput *getFootNode() const { return m_foot; }

        void setStepNode(piranha::pNodeInput stepNode) { m_step = stepNode; }
        const NodeOutput *getStepNode() const { return m_step; }

        void setDcNode(piranha::pNodeInput dcNode) { m_dc = dcNode; }
        const NodeOutput *getDcNode() const { return m_dc; }

        void setDefaultFoot(const math::Vector &v) { m_defaultFoot = v; }
        math::Vector getDefaultFoot() const { return m_defaultFoot; }

        void setDefaultStep(const math::Vector &v) { m_defaultStep = v; }
        math::Vector getDefaultStep() const { return m_defaultStep; }

        void setDefaultDc(const math::Vector &v) { m_defaultDc = v; }
        math::Vector getDefaultDc() const { return m_defaultDc; }

        static math::Vector stepFunction(const math::Vector &input, const math::Vector &dc, 
            const math::Vector &foot, const math::Vector &step);

        piranha::pNodeInput *getInputConnection() { return &m_input; }
        piranha::pNodeInput *getFootConnection() { return &m_foot; }
        piranha::pNodeInput *getStepConnection() { return &m_step; }
        piranha::pNodeInput *getDcConnection() { return &m_dc; }

    protected:
        piranha::pNodeInput m_input;
        piranha::pNodeInput m_foot;
        piranha::pNodeInput m_step;
        piranha::pNodeInput m_dc;
        math::Vector m_defaultFoot;
        math::Vector m_defaultStep;
        math::Vector m_defaultDc;
    };

} /* namespace manta */

#endif /* PIRANHA_STEP_NODE_OUTPUT_H */
