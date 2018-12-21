#include <simple_lens.h>

#include <light_ray.h>
#include <manta_math.h>

manta::SimpleLens::SimpleLens() {
}

manta::SimpleLens::~SimpleLens() {

}

void manta::SimpleLens::setFocus(math::real dist) {
	// Focussing equation: 1/u + 1/v = 1/f
	math::real invFocalLength = (math::real)1.0 / m_lens.calculateFocalLength();
	math::real invFocusDistance = (math::real)1.0 / dist;
	math::real sensorDistance = (math::real)1.0 / (invFocalLength - invFocusDistance);

	m_sensorLocation = math::add(
		m_lens.getPosition(),
		math::mul(math::negate(m_direction), math::loadScalar(sensorDistance)));

	math::real dx = m_sensorWidth / m_sensorResolutionX;
	math::real dy = m_sensorHeight / m_sensorResolutionY;

	m_sensorDeltaX = math::mul(math::loadScalar(dx), m_right);
	m_sensorDeltaY = math::mul(math::loadScalar(dy), m_up);

	m_sensorBottomRight = math::add(
		math::mul(math::loadScalar(-m_sensorWidth / (math::real)2.0), m_right),
		m_sensorLocation);

	m_sensorBottomRight = math::add(
		m_sensorBottomRight,
		math::mul(math::loadScalar(-m_sensorHeight / (math::real)2.0), m_up));
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

	m_right = getSensorRight();
}

manta::math::Vector manta::SimpleLens::getSensorElement(int x, int y) const {
	math::Vector sensorElement = math::add(
		m_sensorBottomRight,
		math::mul(math::loadScalar((math::real)(m_sensorResolutionX - x - 1)), m_sensorDeltaX));
	sensorElement = math::add(
		sensorElement,
		math::mul(math::loadScalar((math::real)y), m_sensorDeltaY));

	return sensorElement;
}

void manta::SimpleLens::lensScan(const math::Vector &sensorElement, LensScanHint *target, int div) const {
	constexpr int MIN_SAMPLES = 4;
	if (div > 128) {
		// Safely assume that no ray can be found
		target->failed = true;
		return;
	}
	
	math::real incr = (m_lens.getRadius() * 2) / (div + 1);

	math::real averageX = (math::real)0.0;
	math::real averageY = (math::real)0.0;
	int samples = 0;

	math::real maxWidth = 0.0;
	bool foundMaxWidth = false;

	for (int i = -div; i < div; i++) {
		math::real maxY = -m_lens.getRadius() * 2;
		math::real minY = m_lens.getRadius() * 2;
		bool detected = false;
		for (int j = -div; j < div; j++) {
			math::real x = incr * i;
			math::real y = incr * j;

			math::Vector target = m_lens.getPosition();
			target = math::add(target, math::mul(math::loadScalar(x), m_right));
			target = math::add(target, math::mul(math::loadScalar(y), m_up));

			math::Vector direction = math::sub(target, sensorElement);
			direction = math::normalize(direction);

			LightRay ray;
			ray.setDirection(direction);
			ray.setSource(sensorElement);

			LightRay targetRay;

			bool success = transformRay(&ray, &targetRay);
			if (success) {
				if (y > maxY) {
					maxY = y;
				}
				if (y < minY) {
					minY = y;
				}

				averageX += x;
				averageY += y;
				samples++;
				detected = true;
			}
		}

		if (detected) {
			math::real width = maxY - minY;
			if (width > maxWidth) {
				maxWidth = width;
				foundMaxWidth = true;
			}
			else if (foundMaxWidth) {
				break;
			}
		}
		else if (foundMaxWidth) {
			break;
		}
	}

	if (samples >= MIN_SAMPLES) {
		target->failed = false;
		target->centerX = averageX / samples;
		target->centerY = averageY / samples;

		math::real safetyRadius = maxWidth / (math::real)2.0 + (math::real)1.5 * incr;
		target->radius = safetyRadius > m_lens.getRadius() ? m_lens.getRadius() : safetyRadius;
	}
	else {
		lensScan(sensorElement, target, div * 2);
	}
}

bool manta::SimpleLens::generateOutgoingRay(const math::Vector &sensorElement, const LensScanHint *hint, LightRay *targetRay) const {
	// Try a max of 100000 times

	math::real maxR = hint->radius;
	
	for (int i = 0; i < 100000; i++) {
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
