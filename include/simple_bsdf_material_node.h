#ifndef MANTARAY_SIMPLE_BSDF_MATERIAL_NODE_H
#define MANTARAY_SIMPLE_BSDF_MATERIAL_NODE_H

#include <piranha.h>

#include "srgb_node_output.h"

namespace manta {

	class SimpleBsdfMaterialNode : public piranha::Node {
	public:
		SimpleBsdfMaterialNode();
		virtual ~SimpleBsdfMaterialNode();

		virtual bool isMaterial() const { return true; }

	protected:
		virtual void _initialize();
		virtual void _evaluate();
		virtual void _destroy();

		virtual void registerOutputs();
		virtual void registerInputs();

	protected:
		piranha::pNodeInput m_name;
        piranha::pNodeInput m_bsdf;
        piranha::pNodeInput m_emission;
        piranha::pNodeInput m_reflectance;
	};

} /* namespace manta */

#endif /* MANTARAY_SIMPLE_BSDF_MATERIAL_NODE_H */
