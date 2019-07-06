#include <material_manager.h>

#include <material.h>
#include <node.h>

manta::MaterialManager::MaterialManager() {
	m_currentIndex = 0;
}

manta::MaterialManager::~MaterialManager() {
	/* void */
}

manta::Material *manta::MaterialManager::searchByName(const std::string &name) const {
	int materialCount = (int)m_materials.size();

	for (int i = 0; i < materialCount; i++) {
		if (m_materials[i]->getName() == name) {
			return m_materials[i];
		}
	}

	return nullptr;
}

void manta::MaterialManager::addMaterialNode(Node *node) {
	m_nodes.push_back(node);
}

void manta::MaterialManager::evaluateAllMaterialNodes() {
	int nodeCount = (int)m_nodes.size();

	for (int i = 0; i < nodeCount; i++) {
		m_nodes[i]->evaluate();
	}
}
