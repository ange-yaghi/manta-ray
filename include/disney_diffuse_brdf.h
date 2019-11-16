#ifndef MANTARAY_DISNEY_DIFFUSE_BRDF_H
#define MANTARAY_DISNEY_DIFFUSE_BRDF_H

#include "bxdf.h"

#include "cacheable_input.h"

namespace manta {

    class VectorMaterialNode;

    class DisneyDiffuseBRDF : public BXDF {
    public:
        DisneyDiffuseBRDF();
        virtual ~DisneyDiffuseBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction,
            const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator);

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator);

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o);

        void setBaseColor(const math::Vector &baseColor) { m_baseColor.setDefault(baseColor); }
        math::Vector getBaseColor() const { return m_baseColor.getDefault(); }

        void setRoughness(math::real roughness) { m_roughness.setDefault(math::loadScalar(roughness)); }
        math::real getRoughness() const { return math::getScalar(m_roughness.getDefault()); }

    protected:
        virtual void registerInputs();

        CacheableInput<math::Vector> m_power;
        CacheableInput<math::Vector> m_baseColor;
        CacheableInput<math::Vector> m_roughness;
    };

} /* namespace manta */

#endif /* MANTARAY_DISNEY_DIFFUSE_BRDF_H */
