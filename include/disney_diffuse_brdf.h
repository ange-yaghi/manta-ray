#ifndef MANTARAY_DISNEY_DIFFUSE_BRDF_H
#define MANTARAY_DISNEY_DIFFUSE_BRDF_H

#include "bxdf.h"

namespace manta {

    class VectorMaterialNode;

    class DisneyDiffuseBRDF : public BXDF {
    public:
        DisneyDiffuseBRDF();
        virtual ~DisneyDiffuseBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator) const;

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) const;

        void setBaseColor(const math::Vector &baseColor) { m_baseColor = baseColor; }
        math::Vector getBaseColor() const { return m_baseColor; }

        void setRoughness(math::real roughness) { m_roughness = roughness; }
        math::real getRoughness() const { return m_roughness; }

    protected:
        math::Vector m_baseColor;
        math::Vector m_power;
        math::real m_roughness;

    protected:
        virtual void registerInputs();

        piranha::pNodeInput m_powerNode;
        piranha::pNodeInput m_baseColorNode;
        piranha::pNodeInput m_roughnessNode;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_DIFFUSE_BRDF_H */
