#ifndef MANTARAY_VECTOR_MAP_2D_NODE_OUTPUT_H
#define MANTARAY_VECTOR_MAP_2D_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "manta_math.h"
#include "vector_map_2d.h"
#include "intersection_point.h"

namespace manta {

    class VectorMap2DNodeOutput : public VectorNodeOutput {
    public:
        static const piranha::ChannelType VectorMap2dType;

    public:
        VectorMap2DNodeOutput();
        virtual ~VectorMap2DNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;
        virtual void fullOutput(const void **target) const;

        const VectorMap2D *getMap() const { return m_map; }
        void setMap(const VectorMap2D *map);

    protected:
        virtual void _evaluateDimensions();

    protected:
        const VectorMap2D *m_map;
    };

} /* namespace manta */

#endif /* MANTARAY_VECTOR_MAP_2D_NODE_OUTPUT_H */
