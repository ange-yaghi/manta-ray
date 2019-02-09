#include <bilayer_bsdf.h>

#include <microfacet_reflection_bsdf.h>
#include <lambertian_bsdf.h>
#include <microfacet_distribution.h>
#include <vector_material_node.h>

#include <iostream>
#include <algorithm>
#include <assert.h>

manta::BilayerBSDF::BilayerBSDF() {
	m_diffuse = math::constants::Zero;
	m_specular = math::constants::Zero;
}

manta::BilayerBSDF::~BilayerBSDF() {

}

manta::math::Vector manta::BilayerBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	math::Vector diffuseR = m_diffuse;
	math::Vector specularR = m_specular;

	if (m_diffuseNode != nullptr) {
		diffuseR = m_diffuseNode->sample(surfaceInteraction);
	}
	
	math::real u = math::uniformRandom();

	math::real coatingPDF;
	math::real diffusePDF;

	math::Vector wh;
	math::real o_dot_m;

	MaterialNodeMemory specularDistMem;
	m_coatingDistribution->initialize(surfaceInteraction, &specularDistMem, stackAllocator);

	if (u < (math::real)0.5) {
		// Ray reflects off of the coating
		math::Vector m = m_coatingDistribution->generateMicrosurfaceNormal(&specularDistMem);
		math::Vector ri = math::reflect(i, m);
		o_dot_m = ::abs(math::getScalar(math::dot(ri, m)));

		*o = ri;

		diffusePDF = (math::real)1.0 / math::constants::TWO_PI;

		if (o_dot_m == 0) {
			coatingPDF = 0.0;
		}
		else {
			coatingPDF = m_coatingDistribution->calculatePDF(*o, m, &specularDistMem) / ((math::real)4 * o_dot_m);
		}

		wh = m;
	}
	else {
		// Ray is transmitted through the coating material
		math::Vector diffuseO;
		m_diffuseMaterial->sampleF(surfaceInteraction, i, &diffuseO, &diffusePDF, stackAllocator);

		*o = diffuseO;

		wh = math::normalize(math::add(*o, i));
		o_dot_m = ::abs(math::getScalar(math::dot(wh, *o)));

		if (o_dot_m == 0) {
			coatingPDF = 0.0;
		}
		else {
			coatingPDF = m_coatingDistribution->calculatePDF(*o, wh, &specularDistMem) / ((math::real)4 * o_dot_m);
		}
	}

	*pdf = (math::real)0.5 * (coatingPDF + diffusePDF);

	// Calculate reflectance
	auto pow5 = [](math::real v) { return (v * v) * (v * v) * v; };

	math::real absCosThetaI = ::abs(math::getZ(i));
	math::real absCosThetaO = ::abs(math::getZ(*o));

	math::Vector diffuse = math::loadScalar((math::real)28.0 / ((math::real)23.0 * math::constants::PI));
	diffuse = math::mul(diffuse, diffuseR);
	diffuse = math::mul(diffuse, math::sub(math::constants::One, specularR));
	diffuse = math::mul(diffuse, math::loadScalar(1 - pow5(1 - (math::real)0.5 * absCosThetaI)));
	diffuse = math::mul(diffuse, math::loadScalar(1 - pow5(1 - (math::real)0.5 * absCosThetaO)));
	
	if (math::getX(wh) == 0 && math::getY(wh) == 0 && math::getZ(wh) == 0){
		// Free all memory
		m_coatingDistribution->free(&specularDistMem, stackAllocator);

		return math::constants::Zero;
	}

	wh = math::normalize(wh);

	math::Vector specular;
	
	math::real absCosThetaOI = std::max(absCosThetaI, absCosThetaO);
	if (o_dot_m == 0 || absCosThetaOI == 0) {
		specular = math::constants::Zero;
	}
	else {
		specular = math::loadScalar(m_coatingDistribution->calculateDistribution(wh, &specularDistMem));
		math::Vector specular_div = math::loadScalar(4 * o_dot_m * absCosThetaOI);
		math::Vector schlickFresnel = math::sub(math::constants::One, specularR);
		schlickFresnel = math::mul(schlickFresnel, math::loadScalar(pow5(1 - absCosThetaO)));
		schlickFresnel = math::add(schlickFresnel, specularR);

		specular = math::div(specular, specular_div);
		specular = math::mul(specular, schlickFresnel);
	}

	// Free all memory
	m_coatingDistribution->free(&specularDistMem, stackAllocator);

	// Return reflectance
	math::Vector fr = math::add(diffuse, specular);
	
	assert(!(::isnan(math::getX(fr)) || ::isnan(math::getY(fr)) || ::isnan(math::getZ(fr))));

	return fr;
}
