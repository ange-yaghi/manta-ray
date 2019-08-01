#ifndef MANTARAY_POWER_NODE_OUTPUT_H
#define MANTARAY_POWER_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "manta_math.h"
#include "vector_map_2d.h"
#include "intersection_point.h"

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

        void setInput(piranha::pNodeInput input) { m_input = input; }
        piranha::pNodeInput getInput() const { return m_input; }

        void setPowerNode(piranha::pNodeInput powerNode) { m_powerNode = powerNode; }
        piranha::pNodeInput getPowerNode() const { return m_powerNode; }

        void setDefaultPower(math::real defaultPower) { m_defaultPower = defaultPower; }
        math::real getDefaultPower() const { return m_defaultPower; }

    protected:
        piranha::pNodeInput m_input;
        piranha::pNodeInput m_powerNode;
        math::real m_defaultPower;
    };

} /* namespace manta */

#endif /* MANTARAY_POWER_NODE_OUTPUT_H */
