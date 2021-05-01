#ifndef MANTARAY_HABLE_FILMIC_NODE_OUTPUT_H
#define MANTARAY_HABLE_FILMIC_NODE_OUTPUT_H

#include "vector_node_output.h"

#include "intersection_point.h"

namespace manta {

    class HableFilmicNodeOutput : public VectorNodeOutput {
    public:
        HableFilmicNodeOutput();
        virtual ~HableFilmicNodeOutput();

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *target) const;
        virtual void discreteSample2d(int x, int y, void *target) const;

        piranha::pNodeInput *getColorConnection() { return &m_color; }
        piranha::pNodeInput *getExposureBiasConnection() { return &m_exposureBias; }
        piranha::pNodeInput *getWhitePointConnection() { return &m_whitePoint; }

        virtual void registerInputs();
        virtual void _evaluateDimensions();

    protected:
        static math::Vector tonemapPartial(const math::Vector &v);
        math::Vector hableFilmic(const math::Vector &color) const;

        piranha::pNodeInput m_whitePoint;
        piranha::pNodeInput m_exposureBias;
        piranha::pNodeInput m_color;
    };

} /* namespace manta */

#endif /* MANTARAY_HABLE_FILMIC_NODE_OUTPUT_H */
