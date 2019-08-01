#ifndef MANTARAY_BILAYER_BSDF_H
#define MANTARAY_BILAYER_BSDF_H

#include "bsdf.h"

#include "lambertian_bsdf.h"
#include "vector_node_output.h"

namespace manta {
	
	class MediaInterface;
    class MicrofacetDistribution;

	class BilayerBSDF : public BSDF {
	public:
		BilayerBSDF();
		virtual ~BilayerBSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, 
			const math::Vector &i, math::Vector *o, math::real *pdf, 
			StackAllocator *stackAllocator) const;

		void setCoatingDistribution(ObjectReferenceNodeOutput<MicrofacetDistribution> *coatingMaterial) 
			{ m_coatingDistribution = coatingMaterial; }
		const piranha::pNodeInput getCoatingDistribution() { return m_coatingDistribution; }

		void setDiffuseMaterial(LambertianBSDF *diffuseMaterial) {}
		//LambertianBSDF *getDiffuseMaterial() { return m_diffuseMaterial; }

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
		LambertianBSDF m_diffuseMaterial;

        piranha::pNodeInput m_diffuseNode;
        piranha::pNodeInput m_specularNode;

		math::Vector m_specular;
		math::Vector m_diffuse;
	};

} /* namespace manta */

#endif /* MANTARAY_BILAYER_BSDF_H */
