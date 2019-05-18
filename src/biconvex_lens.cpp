#include <biconvex_lens.h>

#include <light_ray.h>

manta::BiconvexLens::BiconvexLens() {
	/* void */
}

manta::BiconvexLens::~BiconvexLens() {
	/* void */
}

void manta::BiconvexLens::setInputSurfaceRadius(math::real radius) {
	m_inputSurface.getSphere()->setRadius(radius);
}

void manta::BiconvexLens::setOutputSurfaceRadius(math::real radius) {
	m_outputSurface.getSphere()->setRadius(radius);
}

void manta::BiconvexLens::configure() {
	math::Vector inputLensDisplacement = 
		math::loadScalar(-m_depth / (math::real)2.0 + m_inputSurface.getSphere()->getRadius());
	math::Vector outputLensDisplacement = 
		math::loadScalar(m_depth / (math::real)2.0 - m_inputSurface.getSphere()->getRadius());

	m_inputSurface.getSphere()->setPosition(
		math::add(m_position, math::mul(inputLensDisplacement, m_direction)));
	m_outputSurface.getSphere()->setPosition(
		math::add(m_position, math::mul(outputLensDisplacement, m_direction)));

	m_inputSurface.setIor(m_ior);
	m_outputSurface.setIor(m_ior);
}

bool manta::BiconvexLens::transformLightRay(const LightRay *ray, LightRay *transformed) const {
	LightRay intermediate;
	bool flag;
	math::Vector dist;
	math::real dist_s;

	// Transform at the input surface
	flag = m_inputSurface.transformLightRay(ray, true, true, &intermediate);

	if (!flag) {
		return false;
	}

	// Calculate distance from central axis
	dist = math::sub(m_position, intermediate.getSource());
	dist = math::sub(dist, math::mul(m_direction, math::dot(dist, m_direction)));
	dist = math::magnitude(dist);

	dist_s = math::getScalar(dist);

	if (dist_s > m_radius) {
		return false;
	}

	// Transform at the output surface
	flag = m_outputSurface.transformLightRay(&intermediate, false, false, transformed);

	if (!flag) {
		return false;
	}

	// Calculate distance from central axis
	dist = math::sub(m_position, intermediate.getSource());
	dist = math::sub(dist, math::mul(m_direction, math::dot(dist, m_direction)));
	dist = math::magnitude(dist);

	dist_s = math::getScalar(dist);

	if (dist_s > m_radius) {
		return false;
	}

	return true;
}

bool manta::BiconvexLens::transformLightRayReverse(const LightRay *ray, LightRay *transformed) const {
	LightRay intermediate;
	bool flag;
	math::Vector dist;
	math::real dist_s;

	// Transform at the input surface
	flag = m_outputSurface.transformLightRay(ray, true, true, &intermediate);

	if (!flag) {
		return false;
	}

	// Calculate distance from central axis
	dist = math::sub(m_position, intermediate.getSource());
	dist = math::sub(dist, math::mul(m_direction, math::dot(dist, m_direction)));
	dist = math::magnitude(dist);

	dist_s = math::getScalar(dist);

	if (dist_s > m_radius) {
		return false;
	}

	// Transform at the output surface
	flag = m_inputSurface.transformLightRay(&intermediate, false, false, transformed);

	if (!flag) {
		return false;
	}

	// Calculate distance from central axis
	dist = math::sub(m_position, intermediate.getSource());
	dist = math::sub(dist, math::mul(m_direction, math::dot(dist, m_direction)));
	dist = math::magnitude(dist);

	dist_s = math::getScalar(dist);

	if (dist_s > m_radius) {
		return false;
	}

	return true;
}

manta::math::real manta::BiconvexLens::calculateFocalLength() const {
	math::real R1 = m_inputSurface.getRadius();
	math::real R2 = -m_outputSurface.getRadius();
	math::real d = m_depth;

	math::real inv_f = (m_ior - (math::real)1.0) * 
		((math::real)1.0 / R1 - (math::real)1.0 / R2 + (m_ior - (math::real)1.0) / (m_ior * R1 * R2));

	return (math::real)1.0 / inv_f;
}
