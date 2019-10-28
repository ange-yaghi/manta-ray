#ifndef MANTARAY_DISNEY_SPECULAR_BRDF_H
#define MANTARAY_DISNEY_SPECULAR_BRDF_H

#include "bxdf.h"

namespace manta {

    class VectorMaterialNode;

    class DisneySpecularBRDF : public BXDF {
    public:
        static constexpr math::real MIN_EPSILON = (math::real)0.0;

    public:
        DisneySpecularBRDF();
        virtual ~DisneySpecularBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator) const;

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) const;

        void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

        void setBaseColor(const math::Vector &baseColor) { m_baseColor = baseColor; }
        math::Vector getBaseColor() const { return m_baseColor; }

        void setSpecular(math::real specular) { m_specular = specular; }
        math::real getSpecularColor() const { return m_specular; }

        void setRoughness(math::real roughness) { m_roughness = roughness; }
        math::real getRoughness() const { return m_roughness; }

        void setMetallic(math::real metallic) { m_metallic = metallic; }
        math::real getMetallic() const { return m_metallic; }

        void setPower(math::real power) { m_power = power; }
        math::real getPower() const { return m_power; }

    protected:
        math::Vector m_baseColor;
        math::real m_specular;
        math::real m_roughness;
        math::real m_metallic;
        math::real m_power;

    protected:
        static math::Vector remapSpecular(const math::Vector &specular);

    protected:
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_baseColorNode;
        piranha::pNodeInput m_roughnessNode;
        piranha::pNodeInput m_metallicNode;
        piranha::pNodeInput m_distributionNode;
        piranha::pNodeInput m_specularNode;
        piranha::pNodeInput m_powerNode;

        MicrofacetDistribution *m_distribution;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_SPECULAR_BRDF_H */
