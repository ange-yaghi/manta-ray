#include <bsdf_emitter.h>

#include <bsdf.h>
#include <light_ray.h>

manta::BSDFEmitter::BSDFEmitter() {
	m_samples = 1;

	m_weightLimit = (math::real)2.0;
}

manta::BSDFEmitter::~BSDFEmitter() {
}

void manta::BSDFEmitter::generateRays() {
	initializeRays(m_samples);

	math::Vector u = math::constants::YAxis;
	math::Vector v;
	math::Vector w = m_normal;
	if (abs(math::getX(m_normal)) < 0.1f) {
		u = math::constants::XAxis;
	}
	u = math::normalize(math::cross(u, m_normal));
	v = math::cross(m_normal, u);

	for (int i = 0; i < m_samples; i++) {
		math::Vector m = m_bsdf->generateMicrosurfaceNormal(m_normal, m_incident, u, v);
		math::Vector o = math::reflect(m_incident, m);
		math::real w = m_bsdf->generateWeight(m_normal, m_incident, m, o);

		if (w > m_weightLimit) {
			w = m_weightLimit;
		}

		LightRay &ray = getRays()[i];
		ray.setDirection(o);
		ray.setSource(getPosition());
		ray.setIntensity(math::constants::Zero);
		ray.setWeight(w);
	}
}
