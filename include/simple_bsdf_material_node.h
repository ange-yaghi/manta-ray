#ifndef SIMPLE_BSDF_MATERIAL_NODE_H
#define SIMPLE_BSDF_MATERIAL_NODE_H

#include <node.h>

#include <srgb_node_output.h>

namespace manta {

	class MaterialManager;

	class SimpleBsdfMaterialNode : public Node {
	public:
		SimpleBsdfMaterialNode();
		virtual ~SimpleBsdfMaterialNode();

		void setMaterialManager(MaterialManager *manager) { m_materialManager = manager; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		pNodeInput m_name;
		pNodeInput m_bsdf;
		pNodeInput m_emission;
		pNodeInput m_reflectance;

		MaterialManager *m_materialManager;
	};

} /* namespace manta */

#endif /* SIMPLE_BSDF_MATERIAL_NODE_H */
