#ifndef MANTARAY_PERFECT_SPECULAR_REFLECTION_BSDF_H
#define MANTARAY_PERFECT_SPECULAR_REFLECTION_BSDF_H

#include "bsdf.h"

namespace manta {

    class StackAllocator;

    class PerfectSpecularReflectionBSDF : public BSDF {
    public:
        PerfectSpecularReflectionBSDF();
        virtual ~PerfectSpecularReflectionBSDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;
    };

} /* namespace manta */

#endif /* MANTARAY_PERFECT_SPECULAR_REFLECTION_BSDF_H */