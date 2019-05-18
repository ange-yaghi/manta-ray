#include <simple_lens.h>

#include <light_ray.h>
#include <manta_math.h>

manta::SimpleLens::SimpleLens() {
	/* void */
}

manta::SimpleLens::~SimpleLens() {
	/* void */
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
	/* void */
}

bool manta::SimpleLens::transformRay(const LightRay *inputRay, LightRay *outputRay) const {
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

	outputRay->setAperturePoint(math::Vector2(x, y));

	return true;
}

bool manta::SimpleLens::diffractionRay(const math::Vector2 &aperturePoint, math::Vector direction, math::Vector2 *sensorLocation) const {
	bool flag;

	math::Vector dx = math::mul(math::loadScalar(aperturePoint.x), m_right);
	math::Vector dy = math::mul(math::loadScalar(aperturePoint.y), m_up);

	math::Vector origin = math::add(m_apertureLocation, dx);
	origin = math::add(m_apertureLocation, dy);

	LightRay outgoingRay;
	outgoingRay.setDirection(direction);
	outgoingRay.setSource(origin);

	LightRay finalRay;
	flag = m_lens.transformLightRayReverse(&outgoingRay, &finalRay);
	if (!flag) return false;

	math::Vector dsensor = math::sub(m_sensorLocation, finalRay.getSource());
	math::Vector d1 = math::dot(dsensor, m_direction);
	math::Vector d2 = math::dot(m_direction, finalRay.getDirection());
	math::Vector d = math::div(d1, d2);

	math::Vector intersectionPoint = math::add(math::mul(d, finalRay.getDirection()), finalRay.getSource());
	math::Vector di = math::sub(intersectionPoint, m_sensorLocation);

	math::real x = math::getScalar(math::dot(di, m_right));
	math::real y = math::getScalar(math::dot(di, m_up));

	sensorLocation->x = x;
	sensorLocation->y = y;

	return true;
}

void manta::SimpleLens::initialize() {
	if (m_aperture == nullptr) {
		m_aperture = &m_defaultAperture;
	}
}

void manta::SimpleLens::update() {
	m_lens.setPosition(m_position);
	m_lens.setDirection(m_direction);
	m_lens.setRadius(m_radius);
	m_lens.setIor(LensElement::IOR_CROWN_GLASS);
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

void manta::SimpleLens::lensScan(const math::Vector &sensorElement, 
		int div, math::real span, LensScanHint *target) const {
	lensScan(sensorElement, (math::real)0.0, (math::real)0.0, m_lens.getRadius(), target, div, span);
}

void manta::SimpleLens::lensScan(const math::Vector &sensorElement, 
		math::real offsetX, math::real offsetY, math::real r, LensScanHint *target, int div, math::real span) const {
	constexpr int MIN_SAMPLES = 4;
	if (div > 128) {
		// Safely assume that no ray can be found
		target->failed = true;
		return;
	}
	
	math::real incr = (r * 2) / (div + 1);

	math::real averageX = (math::real)0.0;
	math::real averageY = (math::real)0.0;
	int samples = 0;

	math::real maxWidth = 0.0;

	for (int i = -div; i < div; i++) {
		math::real maxY = -r * 2;
		math::real minY = r * 2;
		bool detected = false;
		for (int j = -div; j < div; j++) {
			math::real x = incr * i + offsetX;
			math::real y = incr * j + offsetY;

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
				if (y > maxY) maxY = y;
				if (y < minY) minY = y;

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
			}
		}
	}

	if (samples >= MIN_SAMPLES) {
		target->failed = false;
		target->centerX = averageX / samples;
		target->centerY = averageY / samples;

		math::real safetyRadius = (math::real)1.5 * maxWidth / (math::real)2.0 + (math::real)1.5 * incr;
		target->radius = safetyRadius > m_lens.getRadius() ? m_lens.getRadius() : safetyRadius;
	}
	else if (samples >= 1) {
		math::real safetyRadius = (math::real)1.5 * maxWidth / (math::real)2.0 + (math::real)1.5 * incr;
		lensScan(sensorElement, averageX / samples, averageY / samples, safetyRadius, target, div, span);
	}
	else {
		lensScan(sensorElement, div * 2, span, target);
	}
}

bool manta::SimpleLens::generateOutgoingRay(const math::Vector &sensorElement, 
		const LensScanHint *hint, LightRay *targetRay) const {
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
