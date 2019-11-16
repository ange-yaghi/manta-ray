#ifndef MANTARAY_LAMBERTIAN_BRDF_H
#define MANTARAY_LAMBERTIAN_BRDF_H

#include "bxdf.h"

namespace manta {

    class VectorMaterialNode;

    struct LambertMemory {
        math::Vector reflectance;
    };

    class LambertianBRDF : public BXDF {
    public:
        LambertianBRDF();
        virtual ~LambertianBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf,
            StackAllocator *stackAllocator);

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator);

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o);
    };

} /* namespace manta */

#endif /* MANTARAY_LAMBERTIAN_BRDF_H */
