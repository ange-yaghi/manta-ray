#include <batched_monte_carlo_emitter.h>

#include <light_ray.h>

manta::BatchedMonteCarloEmitter::BatchedMonteCarloEmitter() {
	m_gloss = 0.0;
}

manta::BatchedMonteCarloEmitter::~BatchedMonteCarloEmitter() {
}

void manta::BatchedMonteCarloEmitter::generateRays() {
	initializeRays(m_samples);

	math::Vector realNormal = math::dot(m_incidentDirection, m_normal);

	if (math::getScalar(realNormal) < 0.0f) {
		realNormal = m_normal;
	}
	else {
		realNormal = math::negate(m_normal);
	}

	math::Vector preferredEmission = m_normal;

	//if (m_gloss >= 1E-5) {
	//	math::Vector perturb = realNormal;
	//	math::Vector n_dot_d = math::dot(realNormal, m_incidentDirection);
	//	perturb = math::mul(perturb, math::add(n_dot_d, n_dot_d)); // Multiply by 2
	//	preferredEmission = math::sub(perturb, m_incidentDirection);
	//	preferredEmission = math::negate(math::normalize(preferredEmission));
	//}

	if (m_gloss + 1E-5 >= (math::real)1.0) {
		// Very simple pure specular emission
		for (int i = 0; i < m_samples; i++) {
			LightRay &ray = getRays()[i];
			ray.setDirection(preferredEmission);
			ray.setSource(getPosition());
			ray.setIntensity(math::constants::Zero);
		}
	}
	else {
		math::Vector u = math::constants::YAxis;
		math::Vector v;
		math::Vector w = realNormal;
		if (abs(math::getX(realNormal)) < 0.1f) {
			u = math::constants::XAxis;
		}
		u = math::normalize(math::cross(u, realNormal));
		v = math::cross(realNormal, u);

		for (int i = 0; i < m_samples; i++) {
			math::real r1 = math::uniformRandom(math::constants::TWO_PI);
			math::real r2 = pow(math::uniformRandom(), 1 - m_gloss);
			math::real r2s = (math::real)sqrt(r2);

			math::Vector diffuse = math::add(
				math::mul(u, math::loadScalar(cos(r1) * r2s)),
				math::mul(v, math::loadScalar(sin(r1) * r2s)));
			diffuse = math::add(
				diffuse,
				math::mul(w, math::loadScalar(sqrt(1 - r2))));
			diffuse = math::normalize(diffuse);

			// Combine specular and diffuse component
			math::real diffuseFactor = (math::real)1.0 - sqrt(m_gloss);

			math::Vector d = math::add(
				math::mul(math::loadScalar(diffuseFactor), diffuse),
				math::mul(math::loadScalar((math::real)1.0 - diffuseFactor), preferredEmission)
			);
			d = math::normalize(d);

			LightRay &ray = getRays()[i];
			ray.setDirection(diffuse);
			ray.setSource(getPosition());
			ray.setIntensity(math::constants::Zero);
		}
	}
}
