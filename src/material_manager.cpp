#include <material_manager.h>

#include <material.h>

manta::MaterialManager::MaterialManager() {
	m_currentIndex = 0;
}

manta::MaterialManager::~MaterialManager() {
	/* void */
}

manta::Material *manta::MaterialManager::searchByName(const std::string &name) const {
	int nMaterials = (int)m_materials.size();

	for (int i = 0; i < nMaterials; i++) {
		if (m_materials[i]->getName() == name) {
			return m_materials[i];
		}
	}

	return nullptr;
}
