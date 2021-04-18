#ifndef MANTARAY_BILAYER_BRDF_H
#define MANTARAY_BILAYER_BRDF_H

#include "bxdf.h"

#include "lambertian_brdf.h"
#include "vector_node_output.h"
#include "cacheable_input.h"

namespace manta {
    
    class MediaInterface;
    class MicrofacetDistribution;

    class BilayerBRDF : public BXDF {
    public:
        BilayerBRDF();
        virtual ~BilayerBRDF();

        virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
            const math::Vector2 &u, const math::Vector &i, math::Vector *o, math::real *pdf,
            RayFlags *flags, StackAllocator *stackAllocator);

        virtual math::Vector f(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o, StackAllocator *stackAllocator);

        virtual math::real pdf(const IntersectionPoint *surfaceInteraction,
            const math::Vector &i, const math::Vector &o);

        void setCoatingDistributionNode(piranha::pNodeInput coatingDistributionNode) 
            { m_coatingDistributionNode = coatingDistributionNode; }
        piranha::pNodeInput getCoatingDistributionNode() { return m_coatingDistributionNode; }

        void setCoatingDistribution(MicrofacetDistribution *distribution);
        MicrofacetDistribution *getCoatingDistribution() const { return m_coatingDistribution; }

        void setSpecularAtNormal(const math::Vector &specular) { m_specular.setDefault(specular); }
        math::Vector getSpecularAtNormal() const { return m_specular.getDefault(); }

        void setDiffuse(const math::Vector &diffuse) { m_diffuse.setDefault(diffuse); }
        math::Vector getDiffuse() const { return m_diffuse.getDefault(); }

        void setDiffuseNode(piranha::pNodeInput diffuseNode) { m_diffuse.setPort(diffuseNode); }
        piranha::pNodeInput getDiffuseNode() const { return m_diffuse.getPort(); }

        void setSpecularNode(piranha::pNodeInput specularNode) { m_specular.setPort(specularNode); }
        piranha::pNodeInput getSpecularNode() const { return m_specular.getPort(); }

    protected:
        virtual void registerInputs();

        virtual void _evaluate();
        piranha::Node *_optimize(piranha::NodeAllocator *nodeAllocator);

    protected:
        MicrofacetDistribution *m_coatingDistribution;

        CacheableInput<math::Vector> m_diffuse;
        CacheableInput<math::Vector> m_specular;
        piranha::pNodeInput m_coatingDistributionNode;
    };

} /* namespace manta */

#endif /* MANTARAY_BILAYER_BRDF_H */
