#ifndef MANTARAY_POWER_NODE_H
#define MANTARAY_POWER_NODE_H

#include <piranha.h>

#include "vector_node_output.h"
#include "power_node_output.h"
#include "manta_math.h"

namespace manta {

    class PowerNode : public piranha::Node {
    public:
        PowerNode();
        PowerNode(math::real power, VectorNodeOutput *input);
        virtual ~PowerNode();

        void setPower(math::real power) { m_power = power; }
        math::real getPower() const { return m_power; }

        void setInputNode(VectorNodeOutput *inputNode) { m_inputNode = inputNode; }
        VectorNodeOutput *getInputNode() const { return m_inputNode; }

        void setPowerNode(VectorNodeOutput *powerNode) { m_powerNode = powerNode; }
        VectorNodeOutput *getPowerNode() const { return m_powerNode; }

        PowerNodeOutput *getMainOutput() { return &m_output; }

    protected:
        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();

        VectorNodeOutput *m_inputNode;
        VectorNodeOutput *m_powerNode;
        PowerNodeOutput m_output;

        math::real m_power;
    };

} /* namespace manta */

#endif /* MANTARAY_POWER_NODE_H */
