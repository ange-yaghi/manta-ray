#ifndef MANTARAY_MATERIAL_POINTER_H
#define MANTARAY_MATERIAL_POINTER_H

#include "material.h"

namespace manta {

    class MaterialPointer : public Material {
    public:
        MaterialPointer();
        virtual ~MaterialPointer();

        void setMaterial(const Material *material) { m_material = material; }
        const Material *getMaterial() const { return m_material; }

    protected:
        const Material *m_material;
    };

} /* namespace manta */

#endif /* MANTARAY_MATERIAL_POINTER_H */
