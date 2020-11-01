#ifndef MANTARAY_DISNEY_SPECULAR_BRDF_H
#define MANTARAY_DISNEY_SPECULAR_BRDF_H

#include "bxdf.h"

#include "cacheable_input.h"

namespace manta {

    class VectorMaterialNode;

    class DisneySpecularBRDF : public BXDF {
    public:
        static constexpr math::real MIN_EPSILON = (math::real)0.0;

    public:
        DisneySpecularBRDF();
        virtual ~DisneySpecularBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction,
            const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator);

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator);

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o);

        void setDistribution(MicrofacetDistribution *distribution) { m_distribution = distribution; }
        const MicrofacetDistribution *getDistribution() const { return m_distribution; }

        void setBaseColor(const math::Vector &baseColor) { m_baseColor.setConstant(baseColor); }
        math::Vector getBaseColor() const { return m_baseColor.getConstant(); }

        void setSpecular(math::real specular) { m_specular.setConstant(math::loadVector(specular)); }
        math::real getSpecularColor() const { return math::getScalar(m_specular.getConstant()); }

        void setRoughness(math::real roughness) { m_roughness.setConstant(math::loadVector(roughness)); }
        math::real getRoughness() const { return math::getScalar(m_roughness.getConstant()); }

        void setPower(math::real power) { m_power.setConstant(math::loadVector(power)); }
        math::real getPower() const { return math::getScalar(m_power.getConstant()); }

    protected:
        static math::Vector remapSpecular(const math::Vector &specular);

    protected:
        virtual void _evaluate();
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
        virtual void registerInputs();

        CacheableInput<math::Vector> m_baseColor;
        CacheableInput<math::Vector> m_roughness;
        CacheableInput<math::Vector> m_specular;
        CacheableInput<math::Vector> m_power;
        piranha::pNodeInput m_distributionNode;

        MicrofacetDistribution *m_distribution;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_SPECULAR_BRDF_H */  
