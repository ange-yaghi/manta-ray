#ifndef MANTARAY_MATERIAL_LIBRARY_H
#define MANTARAY_MATERIAL_LIBRARY_H

#include "object_reference_node.h"

#include <vector>

namespace manta {

    // Forward declarations
    class Material;

    class MaterialLibrary : public ObjectReferenceNode<MaterialLibrary> {
    public:
        MaterialLibrary();
        ~MaterialLibrary();

        template <typename T>
        T *newMaterial() {
            T *newMaterial = new T;
            newMaterial->setIndex(m_currentIndex);
            m_materials.push_back(newMaterial);
            m_currentIndex++;

            return newMaterial;
        }

        void addMaterial(Material *material);

        Material *getMaterial(int index) const { return m_materials[index]; }
        Material *searchByName(const std::string &name) const;

    protected:
        std::vector<Material *> m_materials;
        int m_currentIndex;
    };

} /* namespace manta */

#endif /* MANTARAY_MATERIAL_LIBRARY_H */
