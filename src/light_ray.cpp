#include "../include/light_ray.h"

manta::LightRay::LightRay() {
    m_intensity = math::constants::Zero;
    m_cameraWeight = (math::real)1.0;
    m_kx = m_ky = m_kz -1;
    m_direction = math::constants::Zero;
    m_inverseDirection = math::constants::Zero;
    m_meta = 0;
    m_pdf = (math::real)0;
    m_permutedDirection = math::constants::Zero;
    m_source = math::constants::Zero;
}

manta::LightRay::~LightRay() {
    /* void */
}

void manta::LightRay::calculateTransformations() {
    m_kz = math::maxDimension3(math::abs(m_direction));
    m_kx = (m_kz + 1) % 3;
    m_ky = (m_kx + 1) % 3;

    m_permutedDirection = math::permute(m_direction, m_kx, m_ky, m_kz);

    const math::real dirZ = math::getZ(m_permutedDirection);
    m_shear.x = -math::getX(m_permutedDirection) / dirZ;
    m_shear.y = -math::getY(m_permutedDirection) / dirZ;
    m_shear.z = (math::real)1.0 / dirZ;

    m_inverseDirection = math::div(math::constants::One, m_direction);
}
