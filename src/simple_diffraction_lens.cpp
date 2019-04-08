#include <simple_diffraction_lens.h>

#include <light_ray.h>
#include <manta_math.h>

manta::SimpleDiffractionLens::SimpleDiffractionLens() {
	m_diffractionPlaneZ = (math::real)1.0;
}

manta::SimpleDiffractionLens::~SimpleDiffractionLens() {

}

bool manta::SimpleDiffractionLens::transformRay(const LightRay *inputRay, LightRay *outputRay) const {
	bool flag;

	flag = m_lens.transformLightRay(inputRay, outputRay);
	if (!flag) return false;

	math::Vector d = math::sub(outputRay->getSource(), m_apertureLocation);
	math::Vector proj_x = math::dot(d, m_right);
	math::Vector proj_y = math::dot(d, m_up);

	math::real x, y;
	x = math::getScalar(proj_x);
	y = math::getScalar(proj_y);

	flag = m_aperture->filter(x, y);
	if (!flag) return false;

	math::Vector dot_z = math::dot(outputRay->getDirection(), m_direction);
	math::Vector diffractionObservationPoint = math::sub(
		outputRay->getSource(), 
		math::mul(
			math::div(
				math::loadScalar(m_diffractionPlaneZ), 
				dot_z), 
			outputRay->getDirection()));

	math::Vector d_diff = math::sub(diffractionObservationPoint, m_diffractionPlaneLocation);

	math::real diff_x, diff_y;
	diff_x = math::getScalar(math::dot(d_diff, m_right));
	diff_y = math::getScalar(math::dot(d_diff, m_up));

	math::real diffSensorWidth = m_diffractionModel.getPhysicalSensorWidth();

	math::real diffractionPlaneWidth = m_aperture->getRadius() * (math::real)1.0;
	math::real incidentLight_x; 
	math::real incidentLight_y;
	math::real pdfDiffraction, pdfNormal, pdf;
	math::real diffractionSampleProb = (math::real)0.5;

	pdfDiffraction = 1 / (4 * diffractionPlaneWidth * diffractionPlaneWidth);

	if (math::uniformRandom() < (math::real)diffractionSampleProb) {
		incidentLight_x = (1 - 2 * math::uniformRandom()) * diffractionPlaneWidth;
		incidentLight_y = (1 - 2 * math::uniformRandom()) * diffractionPlaneWidth;

		pdfNormal = 0.0;
	}
	else {
		incidentLight_x = diff_x;
		incidentLight_y = diff_y;

		pdfNormal = 1.0;
	}

	pdf = (diffractionSampleProb * pdfDiffraction + (1 - diffractionSampleProb) * pdfNormal);

	math::Vector newOrigin = math::add(
		math::mul(math::loadScalar(incidentLight_x), m_right),
		math::mul(math::loadScalar(incidentLight_y), m_up)
	);
	newOrigin = math::add(newOrigin, m_diffractionPlaneLocation);

	math::Vector aperturePoint = math::mul(proj_x, m_right);
	aperturePoint = math::add(aperturePoint, math::mul(proj_y, m_up));
	aperturePoint = math::add(aperturePoint, m_apertureLocation);

	math::Vector newTarget = aperturePoint;
	math::Vector newDirection = math::normalize(math::sub(newTarget, newOrigin));

	math::Vector diffractionPattern;
	diffractionPattern = m_diffractionModel.samplePattern(diff_x - incidentLight_x, diff_y - incidentLight_y);
	diffractionPattern = math::mul(diffractionPattern, math::loadScalar(1.1 / pdf));

	outputRay->setAperturePoint(math::Vector2(x, y));
	outputRay->setDirection(newDirection);
	outputRay->setSource(newOrigin);
	outputRay->setWeight(diffractionPattern);

	return true;
}

bool manta::SimpleDiffractionLens::generateOutgoingRay(const math::Vector &sensorElement, const LensScanHint *hint, LightRay *targetRay) const {
	constexpr int MAX_ATTEMPTS = 100000;

	math::real maxR = hint->radius;

	// Try to find a suitable outgoing ray MAX_ATTEMPTS times
	for (int i = 0; i < MAX_ATTEMPTS; i++) {
		math::real randTheta = math::uniformRandom(math::constants::TWO_PI);
		math::real randR = math::uniformRandom(1.0);

		math::real randX = cos(randTheta) * sqrt(randR) * maxR + hint->centerX;
		math::real randY = sin(randTheta) * sqrt(randR) * maxR + hint->centerY;

		math::Vector target = m_lens.getPosition();
		target = math::add(target, math::mul(math::loadScalar(randX), m_right));
		target = math::add(target, math::mul(math::loadScalar(randY), m_up));

		math::Vector direction = math::sub(target, sensorElement);
		direction = math::normalize(direction);

		LightRay ray;
		ray.setDirection(direction);
		ray.setSource(sensorElement);

		bool success = transformRay(&ray, targetRay);
		if (success) return true;
	}

	return false;
}

void manta::SimpleDiffractionLens::update() {
	m_lens.setPosition(m_position);
	m_lens.setDirection(m_direction);
	m_lens.setRadius(m_radius);
	m_lens.setIOR(LensElement::IOR_CROWN_GLASS);
	m_lens.setInputSurfaceRadius((math::real)10.0);
	m_lens.setOutputSurfaceRadius((math::real)10.0);
	m_lens.setDepth((math::real)0.1);
	m_lens.configure();

	// Calculate the location of the aperture
	m_apertureLocation = m_position;
	m_apertureLocation = math::add(
		m_apertureLocation,
		math::mul(
			math::loadScalar(m_lens.getDepth() / (math::real)2.0),
			m_direction));

	m_diffractionPlaneLocation = math::sub(
		m_apertureLocation,
		math::mul(
			math::loadScalar(m_diffractionPlaneZ),
			m_direction));

	m_right = getSensorRight();
}
