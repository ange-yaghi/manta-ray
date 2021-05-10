#ifndef MANTARAY_SPECULAR_GLASS_BSDF_H
#define MANTARAY_SPECULAR_GLASS_BSDF_H

#include "bxdf.h"

namespace manta {

    class SpecularGlassBSDF : public BXDF {
    public:
        SpecularGlassBSDF();
        virtual ~SpecularGlassBSDF();

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

        void setTint(const math::Vector &tint) { m_tint.setConstant(tint); }
        math::Vector getTint() const { return m_tint.getConstant(); }

        void setMediaInterface(MediaInterface *mediaInterface) { m_mediaInterface = mediaInterface; }
        MediaInterface *getMediaInterface() { return m_mediaInterface; }

    protected:
        virtual piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);
        virtual void _evaluate();

        virtual void registerInputs();

        CacheableInput<math::Vector> m_tint;
        CacheableInput<math::Vector> m_reflectionBias;
        MediaInterface *m_mediaInterface;

        piranha::pNodeInput m_mediaInterfaceInput;
    };

} /* namespace manta */

#endif /* MANTARAY_SPECULAR_GLASS_BSDF_H */
