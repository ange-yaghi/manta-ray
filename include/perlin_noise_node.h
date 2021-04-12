#ifndef MANTARAY_PERLIN_NOISE_NODE_H
#define MANTARAY_PERLIN_NOISE_NODE_H

#include <piranha.h>

#include "perlin_noise_node_output.h"
#include "manta_math.h"

namespace manta {

    class PerlinNoiseNode : public piranha::Node {
    public:
        PerlinNoiseNode();
        virtual ~PerlinNoiseNode();

    protected:
        PerlinNoiseNodeOutput m_output;

        virtual void _initialize();

        virtual void registerInputs();
        virtual void registerOutputs();
    };

} /* namespace manta */

#endif /* MANTARAY_PERLIN_NOISE_NODE_H */
