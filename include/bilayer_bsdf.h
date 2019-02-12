#ifndef BILAYER_BSDF_H
#define BILAYER_BSDF_H

#include <bsdf.h>

namespace manta {
	
	class MediaInterface;
	class LambertianBSDF;
	class MicrofacetDistribution;
	class VectorMaterialNode;

	class BilayerBSDF : public BSDF {
	public:
		BilayerBSDF();
		~BilayerBSDF();

		virtual math::Vector sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const;

		void setCoatingDistribution(MicrofacetDistribution *coatingMaterial) { m_coatingDistribution = coatingMaterial; }
		MicrofacetDistribution *getCoatingDistribution() { return m_coatingDistribution; }

		void setDiffuseMaterial(LambertianBSDF *diffuseMaterial) { m_diffuseMaterial = diffuseMaterial; }
		LambertianBSDF *getDiffuseMaterial() { return m_diffuseMaterial; }

		void setSpecularAtNormal(const math::Vector &specular) { m_specular = specular; }
		math::Vector getSpecularAtNormal() const { return m_specular; }

		void setDiffuse(const math::Vector &diffuse) { m_diffuse = diffuse; }
		math::Vector getDiffuse() const { return m_diffuse; }

		void setDiffuseNode(VectorMaterialNode *diffuseNode) { m_diffuseNode = diffuseNode; }
		VectorMaterialNode *getDiffuseNode() const { return m_diffuseNode; }

		void setSpecularNode(VectorMaterialNode *specularNode) { m_specularNode = specularNode; }
		VectorMaterialNode *getSpecularNode() const { return m_specularNode; }

	protected:
		MicrofacetDistribution *m_coatingDistribution;
		LambertianBSDF *m_diffuseMaterial;

		VectorMaterialNode *m_diffuseNode;
		VectorMaterialNode *m_specularNode;

		math::Vector m_specular;
		math::Vector m_diffuse;
	};

} /* namespace manta */

#endif /* BILAYER_BSDF_H */
