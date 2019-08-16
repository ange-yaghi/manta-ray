#ifndef MANTARAY_POLYGONAL_APERTURE_H
#define MANTARAY_POLYGONAL_APERTURE_H

#include "aperture.h"

namespace manta {

    class PolygonalAperture : public Aperture {
    protected:
        struct Edge {
            math::Vector2 reference;
            math::Vector2 direction;
            math::real cache;
        };

    public:
        PolygonalAperture();
        virtual ~PolygonalAperture();

        void configure(int edges, math::real angle = (math::real)0.0, bool halfOffset=false);
        void destroy();

        virtual bool filter(math::real x, math::real y) const;

        void setBladeCurvature(math::real bladeCurvature) { m_bladeCurvature = bladeCurvature; }
        math::real getBladeCurvature() const { return m_bladeCurvature; }

    protected:
        Edge *m_edges;
        int m_edgeCount;

        math::real m_bladeCurvature;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerInputs();

        piranha::pNodeInput m_edgeCountInput;
        piranha::pNodeInput m_bladeCurvatureInput;
        piranha::pNodeInput m_angleInput;
    };

} /* namespace manta */

#endif /* MANTARAY_POLYGONAL_APERTURE_H */
