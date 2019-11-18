#include "../include/lens_camera_ray_emitter.h"

#include "../include/light_ray.h"
#include "../include/sampler.h"
#include "../include/stack_allocator.h"
#include "../include/ray_container.h"

manta::LensCameraRayEmitter::LensCameraRayEmitter() {
    m_lens = nullptr;
}

manta::LensCameraRayEmitter::~LensCameraRayEmitter() {
    /* void */
}

void manta::LensCameraRayEmitter::initialize() {
    m_lens->lensScan(m_imagePlaneOrigin, 4, m_pixelIncrement.x, &m_lensHint);
}

void manta::LensCameraRayEmitter::generateRay(LightRay *ray) const {
    const math::Vector2 samplePoint = m_sampler->generate2d();
    math::Vector position = transformToImagePlane(
        math::Vector2(samplePoint.x - (math::real)0.5, samplePoint.y - (math::real)0.5));

    ray->setIntensity(math::constants::Zero);
    ray->setWeight(math::constants::One);

    bool result = m_lens->generateOutgoingRay(position, &m_lensHint, ray);

    ray->setImagePlaneLocation(
        math::Vector2(
            -(samplePoint.x - (math::real)0.5) + (math::real)m_pixelX,
            (samplePoint.y - (math::real)0.5) + (math::real)m_pixelY));
}
