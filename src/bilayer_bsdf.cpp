#include <bilayer_bsdf.h>

#include <microfacet_reflection_bsdf.h>
#include <lambertian_bsdf.h>
#include <microfacet_distribution.h>
#include <vector_material_node.h>

#include <iostream>
#include <algorithm>
#include <assert.h>

manta::BilayerBSDF::BilayerBSDF() {
	m_diffuseNode = nullptr;
	m_specularNode = nullptr;

	m_diffuse = math::constants::One;
	m_specular = math::constants::Zero;
}

manta::BilayerBSDF::~BilayerBSDF() {

}

manta::math::Vector manta::BilayerBSDF::sampleF(const IntersectionPoint *surfaceInteraction, const math::Vector &i, math::Vector *o, math::real *pdf, StackAllocator *stackAllocator) const {
	math::Vector diffuseR = m_diffuse;
	math::Vector specularR = m_specular;

	if (m_diffuseNode != nullptr) {
		diffuseR = math::mul(m_diffuseNode->sample(surfaceInteraction), diffuseR);
	}

	if (m_specularNode != nullptr) {
		specularR = math::mul(m_specularNode->sample(surfaceInteraction), specularR);
	}
	
	math::real u = math::uniformRandom();

	math::real coatingPDF;
	math::real diffusePDF;

	math::Vector wh;

	MaterialNodeMemory specularDistMem;
	m_coatingDistribution->initialize(surfaceInteraction, &specularDistMem, stackAllocator);

	if (u < (math::real)0.5) {
		// Ray reflects off of the coating
		math::Vector m = m_coatingDistribution->generateMicrosurfaceNormal(&specularDistMem);
		math::Vector ri = math::reflect(i, m);
		wh = m;
		*o = ri;
	}
	else {
		// Ray is transmitted through the coating material
		math::Vector diffuseO;
		m_diffuseMaterial->sampleF(surfaceInteraction, i, &diffuseO, &diffusePDF, stackAllocator);
		wh = math::add(*o, i);
		*o = diffuseO;
	}
	
	if (math::getX(wh) == 0 && math::getY(wh) == 0 && math::getZ(wh) == 0) {
		// Free all memory
		m_coatingDistribution->free(&specularDistMem, stackAllocator);

		*pdf = 0.0;
		return math::constants::Zero;
	}

	wh = math::normalize(wh);

	math::real cosThetaO = math::getZ(*o);
	math::real cosThetaI = math::getZ(i);

	if (cosThetaO <= (math::real)0.0 || cosThetaI <= (math::real)0.0) {
		*pdf = 0.0;
		return math::constants::Zero;
	}

	math::real o_dot_wh;
	math::real abs_o_dot_wh;
	o_dot_wh = math::getScalar(math::dot(wh, *o));
	abs_o_dot_wh = ::abs(o_dot_wh);

	if (o_dot_wh <= 0 || math::getZ(wh) < 0) {
		coatingPDF = 0.0;
	}
	else {
		coatingPDF = m_coatingDistribution->calculatePDF(wh, &specularDistMem) / ((math::real)4 * o_dot_wh);
	}

	diffusePDF = (math::real)1.0 / math::constants::TWO_PI;

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

	math::Vector specular;
	
	math::real absCosThetaOI = std::max(absCosThetaI, absCosThetaO);
	if (o_dot_wh <= 0 || absCosThetaOI == 0 || math::getZ(wh) < 0) {
		specular = math::constants::Zero;
	}
	else {
		specular = math::loadScalar(m_coatingDistribution->calculateDistribution(wh, &specularDistMem));
		if (math::getX(specular) < 0 || math::getY(specular) < 0 || math::getZ(specular) < 0) {
			//std::cout << math::getZ(*o) << "," << math::getZ(i) << "," << math::getZ(wh) << std::endl;
		}

		math::Vector specular_div = math::loadScalar(4 * abs_o_dot_wh * absCosThetaOI);
		math::Vector schlickFresnel = math::sub(math::constants::One, specularR);
		schlickFresnel = math::mul(schlickFresnel, math::loadScalar(pow5(1 - o_dot_wh)));
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
