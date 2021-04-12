#ifndef MANTARAY_TURBULENCE_NOISE_NODE_H
#define MANTARAY_TURBULENCE_NOISE_NODE_H

#include <piranha.h>

#include "turbulence_noise_node_output.h"
#include "manta_math.h"

namespace manta {

    class TurbulenceNoiseNode : public piranha::Node {
    public:
        TurbulenceNoiseNode();
        virtual ~TurbulenceNoiseNode();

    protected:
        TurbulenceNoiseNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_TURBULENCE_NOISE_NODE_H */
