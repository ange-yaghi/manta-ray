#ifndef MANTARAY_BILAYER_BRDF_H
#define MANTARAY_BILAYER_BRDF_H

#include "bxdf.h"

#include "lambertian_brdf.h"
#include "vector_node_output.h"

namespace manta {
    
    class MediaInterface;
    class MicrofacetDistribution;

    class BilayerBRDF : public BXDF {
    public:
        BilayerBRDF();
        virtual ~BilayerBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector &i, math::Vector *o, math::real *pdf, 
            StackAllocator *stackAllocator) const;

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator) const;

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o) const;

        void setCoatingDistribution(ObjectReferenceNodeOutput<MicrofacetDistribution> *coatingMaterial) 
            { m_coatingDistribution = coatingMaterial; }
        const piranha::pNodeInput getCoatingDistribution() { return m_coatingDistribution; }

        void setSpecularAtNormal(const math::Vector &specular) { m_specular = specular; }
        math::Vector getSpecularAtNormal() const { return m_specular; }

        void setDiffuse(const math::Vector &diffuse) { m_diffuse = diffuse; }
        math::Vector getDiffuse() const { return m_diffuse; }

        void setDiffuseNode(piranha::pNodeInput diffuseNode) { m_diffuseNode = diffuseNode; }
        piranha::pNodeInput getDiffuseNode() const { return m_diffuseNode; }

        void setSpecularNode(piranha::pNodeInput specularNode) { m_specularNode = specularNode; }
        piranha::pNodeInput getSpecularNode() const { return m_specularNode; }

    protected:
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_coatingDistribution;
        LambertianBRDF m_diffuseMaterial;

        piranha::pNodeInput m_diffuseNode;
        piranha::pNodeInput m_specularNode;

        math::Vector m_specular;
        math::Vector m_diffuse;
    };

} /* namespace manta */

#endif /* MANTARAY_BILAYER_BRDF_H */
