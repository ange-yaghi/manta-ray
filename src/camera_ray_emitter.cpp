#include "../include/camera_ray_emitter.h"

#include "../include/light_ray.h"

manta::CameraRayEmitter::CameraRayEmitter() {
    m_sampler = nullptr;

    m_position = math::constants::Zero;
    m_direction = math::constants::Zero;
    m_sampleCount = 0;
}

manta::CameraRayEmitter::~CameraRayEmitter() {
    /* void */
}

void manta::CameraRayEmitter::initialize() {
    /* void */
}

manta::math::Vector manta::CameraRayEmitter::transformToImagePlane(const math::Vector2 coordinates) const {
    math::Vector u = math::mul(m_right, math::loadScalar(coordinates.x * m_pixelIncrement.x));
    math::Vector v = math::mul(m_up, math::loadScalar(coordinates.y * m_pixelIncrement.y));
    math::Vector samplePoint3d = math::add(u, v);

    math::Vector imagePlaneT = math::add(m_imagePlaneOrigin, samplePoint3d);

    return imagePlaneT;
}
