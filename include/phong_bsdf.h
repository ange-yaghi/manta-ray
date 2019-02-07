#ifndef PHONG_BSDF_H
#define PHONG_BSDF_H

#include <bsdf.h>

#include <phong_distribution.h>

namespace manta {

	class VectorMaterialNode;

	class PhongBSDF : public BSDF {
	public:
		PhongBSDF();
		~PhongBSDF();

		virtual void initialize(BSDFInput *bsdfInput, StackAllocator *s) const;

		virtual math::Vector generateMicrosurfaceNormal(const BSDFInput &bsdfInput) const;
		virtual math::real generateWeight(const BSDFInput &bsdfInput, const math::Vector &m, const math::Vector &o) const;

		void setPower(math::real power) { m_power = power; }
		math::real getPower() const { return m_power; }

		void setPowerNode(VectorMaterialNode *node) { m_powerNode = node; }
		VectorMaterialNode *getPowerNode() const { return m_powerNode; }

	protected:
		virtual math::real g1(const BSDFInput &bsdfInput, const math::Vector &v, const math::Vector &m) const;

		VectorMaterialNode *m_powerNode;
		math::real m_power;
	};

} /* namespace manta */

#endif /* PHONG_BSDF_H */
