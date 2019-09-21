#ifndef MANTARAY_SIMPLE_BSDF_MATERIAL_H
#define MANTARAY_SIMPLE_BSDF_MATERIAL_H

#include "material.h"

#include "vector_node_output.h"

namespace manta {

    // Forward declarations
    class BSDF;

    class SimpleBSDFMaterial : public Material {
    public:
        SimpleBSDFMaterial();
        virtual ~SimpleBSDFMaterial();

        void setMaxDegree(int degree) { m_maxDegree = degree; }
        int getMaxDegree() const { return m_maxDegree; }

        void setReflectanceNode(VectorNodeOutput *node) { m_reflectanceNode = node; }
        const VectorNodeOutput *getReflectanceNode() const;

        void setEmissionNode(VectorNodeOutput *node) { m_emissionNode = node; }
        const VectorNodeOutput *getEmissionNode() const;

        void setEmission(const math::Vector &emission) { m_emission = emission; }
        math::Vector getEmission() { return m_emission; }

        void setReflectance(const math::Vector &reflectance) { m_reflectance = reflectance; }
        math::Vector getReflectance() { return m_reflectance; }

        void setBSDF(BSDF *bsdf) { m_bsdf = bsdf; }
        const BSDF *getDefaultBSDF() const { return m_defaultBsdf; }

        virtual math::Vector getFilterColor(const IntersectionPoint &ip) const;
        virtual math::Vector getEmission(const IntersectionPoint &ip) const;
        virtual BSDF *getBSDF() const { return m_bsdf; }

    protected:
        virtual void _evaluate();
        virtual void _initialize();
        virtual void registerInputs();

        int m_maxDegree;

        piranha::pNodeInput m_reflectanceNode;
        piranha::pNodeInput m_emissionNode;

        // Single BSDF
        piranha::pNodeInput m_bsdfInput;

        // Fixed parameters
        BSDF *m_bsdf;
        math::Vector m_emission;
        math::Vector m_reflectance;
        const BSDF *m_defaultBsdf;
    };

} /* namespace manta */

#endif /* MANTARAY_SIMPLE_BSDF_MATERIAL_H */
