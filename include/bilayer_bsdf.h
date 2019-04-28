#ifndef BILAYER_BSDF_H
#define BILAYER_BSDF_H

#include <bsdf.h>

#include <vector_node_output.h>

namespace manta {
	
	class MediaInterface;
	class LambertianBSDF;
	class MicrofacetDistributionNodeOutput;

	class BilayerBSDF : public BSDF {
	public:
		BilayerBSDF();
		~BilayerBSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const;

		void setCoatingDistribution(const MicrofacetDistributionNodeOutput *coatingMaterial) { m_coatingDistribution = coatingMaterial; }
		const MicrofacetDistributionNodeOutput *getCoatingDistribution() { return m_coatingDistribution; }

		void setDiffuseMaterial(LambertianBSDF *diffuseMaterial) { m_diffuseMaterial = diffuseMaterial; }
		LambertianBSDF *getDiffuseMaterial() { return m_diffuseMaterial; }

		void setSpecularAtNormal(const math::Vector &specular) { m_specular = specular; }
		math::Vector getSpecularAtNormal() const { return m_specular; }

		void setDiffuse(const math::Vector &diffuse) { m_diffuse = diffuse; }
		math::Vector getDiffuse() const { return m_diffuse; }

		void setDiffuseNode(pNodeInput diffuseNode) { m_diffuseNode = diffuseNode; }
		pNodeInput getDiffuseNode() const { return m_diffuseNode; }

		void setSpecularNode(pNodeInput specularNode) { m_specularNode = specularNode; }
		pNodeInput getSpecularNode() const { return m_specularNode; }

	protected:
		virtual void registerInputs();

	protected:
		const MicrofacetDistributionNodeOutput *m_coatingDistribution;
		LambertianBSDF *m_diffuseMaterial;

		pNodeInput m_diffuseNode;
		pNodeInput m_specularNode;

		math::Vector m_specular;
		math::Vector m_diffuse;
	};

} /* namespace manta */

#endif /* BILAYER_BSDF_H */
