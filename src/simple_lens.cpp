#include <simple_lens.h>

#include <light_ray.h>

manta::SimpleLens::SimpleLens() {
}

manta::SimpleLens::~SimpleLens() {

}

void manta::SimpleLens::setFocus(math::real dist) {
	// Focussing equation: 1/u + 1/v = 1/f
	math::real invFocalLength = (math::real)1.0 / m_lens.calculateFocalLength();
	math::real invFocusDistance = (math::real)1.0 / dist;
	math::real sensorDistance = (math::real)1.0 / (invFocalLength - invFocusDistance);

	m_sensorLocation = math::mul(math::negate(m_direction), math::loadScalar(sensorDistance));
}

void manta::SimpleLens::setFocalLength(math::real focalLength) {

}

bool manta::SimpleLens::transformRay(const LightRay *inputRay, LightRay *outputRay) const {
	bool flag;

	flag = m_lens.transformLightRay(inputRay, outputRay);

	if (!flag) return false;

	// Calculate the location of the aperture
	math::Vector apertureLocation = m_position;
	apertureLocation = math::add(
		apertureLocation,
		math::mul(
			math::loadScalar(m_lens.getDepth() / (math::real)2.0),
			m_direction));

	math::Vector right = getSensorRight();
	math::Vector d = math::sub(outputRay->getSource(), apertureLocation);
	math::Vector proj_x = math::mul(math::dot(d, right), right);
	math::Vector proj_y = math::mul(math::dot(d, m_up), m_up);

	math::real x, y;
	x = math::getScalar(math::magnitude(proj_x));
	y = math::getScalar(math::magnitude(proj_y));

	flag = m_aperture.filter(x, y);

	if (!flag) return false;

	return true;
}

void manta::SimpleLens::initialize() {
	setAperture((Aperture *)&m_aperture);
}

void manta::SimpleLens::update() {
	m_lens.setPosition(m_position);
	m_lens.setDirection(m_direction);
	m_lens.setRadius(m_radius);
	m_lens.setIOR(LensElement::IOR_CROWN_GLASS);
	m_lens.setInputSurfaceRadius((math::real)10.0);
	m_lens.setOutputSurfaceRadius((math::real)10.0);
	m_lens.setDepth((math::real)0.1);
	m_lens.configure();
}
