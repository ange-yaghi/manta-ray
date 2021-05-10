#ifndef MANTARAY_LAMBERTIAN_BRDF_H
#define MANTARAY_LAMBERTIAN_BRDF_H

#include "bxdf.h"

namespace manta {

    class VectorMaterialNode;
    class LambertianBRDF : public BXDF {
    public:
        LambertianBRDF();
        virtual ~LambertianBRDF();

        virtual math::Vector sampleF(
            const IntersectionPoint *surfaceInteraction, 
            const math::Vector2 &u,
            const math::Vector &i,
            math::Vector *o,
            math::real *pdf,
            RayFlags *flags,
            StackAllocator *stackAllocator);

        virtual math::Vector f(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector &i,
            const math::Vector &o,
            StackAllocator *stackAllocator);

        virtual math::real pdf(
            const IntersectionPoint *surfaceInteraction,
            const math::Vector &i,
            const math::Vector &o);

    protected:
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
        virtual void registerInputs();

        CacheableInput<math::Vector> m_color;
    };

} /* namespace manta */

#endif /* MANTARAY_LAMBERTIAN_BRDF_H */
