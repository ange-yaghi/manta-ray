#include <light_ray.h>

manta::LightRay::LightRay() {
	m_intensity = math::constants::Zero;
	m_weight = (math::real)1.0;
}

manta::LightRay::~LightRay() {

}
