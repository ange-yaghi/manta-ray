#include <simple_bsdf_material_node.h>

#include <simple_bsdf_material.h>
#include <material_manager.h>
#include <bsdf_node_output.h>

manta::SimpleBsdfMaterialNode::SimpleBsdfMaterialNode() {
	/* void */
}

manta::SimpleBsdfMaterialNode::~SimpleBsdfMaterialNode() {
	/* void */
}

void manta::SimpleBsdfMaterialNode::_initialize() {
	/* void */
}

void manta::SimpleBsdfMaterialNode::_evaluate() {
	SimpleBSDFMaterial *newMaterial = m_materialManager->newMaterial<SimpleBSDFMaterial>();

	BSDF *bsdf;
	std::string name;

	bsdf = ((BSDFNodeOutput *)m_bsdf)->getBsdf();
	m_name->fullCompute((void *)&name);

	newMaterial->setName(name);
	newMaterial->setBSDF(bsdf);
	newMaterial->setEmissionNode((const VectorNodeOutput *)m_emission);
	newMaterial->setReflectanceNode((const VectorNodeOutput *)m_reflectance);
}

void manta::SimpleBsdfMaterialNode::_destroy() {
	/* void */
}

void manta::SimpleBsdfMaterialNode::registerOutputs() {
	/* void */
}

void manta::SimpleBsdfMaterialNode::registerInputs() {
	registerInput(&m_name, "name");
	registerInput(&m_bsdf, "bsdf");
	registerInput(&m_reflectance, "reflectance");
	registerInput(&m_emission, "emission");
}
