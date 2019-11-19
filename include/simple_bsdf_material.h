#ifndef MANTARAY_SIMPLE_BSDF_MATERIAL_H
#define MANTARAY_SIMPLE_BSDF_MATERIAL_H

#include "material.h"

#include "vector_node_output.h"
#include "cacheable_input.h"

namespace manta {

    // Forward declarations
    class BSDF;

    class SimpleBSDFMaterial : public Material {
    public:
        SimpleBSDFMaterial();
        virtual ~SimpleBSDFMaterial();

        void setMaxDegree(int degree) { m_maxDegree = degree; }
        int getMaxDegree() const { return m_maxDegree; }

        void setReflectanceNode(VectorNodeOutput *node) { m_reflectance.setPort(node); }
        const VectorNodeOutput *getReflectanceNode() const;

        void setEmissionNode(VectorNodeOutput *node) { m_emission.setPort(node); }
        const VectorNodeOutput *getEmissionNode() const;

        void setEmission(const math::Vector &emission) { m_emission.setDefault(emission); }
        math::Vector getEmission() { return m_emission.getDefault(); }

        void setReflectance(const math::Vector &reflectance) { m_reflectance.setDefault(reflectance); }
        math::Vector getReflectance() { return m_reflectance.getDefault(); }

        void setBSDF(BSDF *bsdf) { m_bsdf = bsdf; }
        const BSDF *getDefaultBSDF() const { return m_defaultBsdf; }

        virtual math::Vector getFilterColor(const IntersectionPoint &ip);
        virtual math::Vector getEmission(const IntersectionPoint &ip);
        virtual BSDF *getBSDF() const { return m_bsdf; }

    protected:
        virtual void _evaluate();
        virtual void _initialize();
        virtual void registerInputs();

        int m_maxDegree;

        // Single BSDF
        piranha::pNodeInput m_bsdfInput;

        // Fixed parameters
        BSDF *m_bsdf;
        CacheableInput<math::Vector> m_emission;
        CacheableInput<math::Vector> m_reflectance;
        const BSDF *m_defaultBsdf;
    };

} /* namespace manta */

#endif /* MANTARAY_SIMPLE_BSDF_MATERIAL_H */
