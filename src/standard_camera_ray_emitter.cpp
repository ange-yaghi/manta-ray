#include "../include/standard_camera_ray_emitter.h"

#include "../include/sampler.h"
#include "../include/light_ray.h"
#include "../include/stack_allocator.h"
#include "../include/ray_container.h"

manta::StandardCameraRayEmitter::StandardCameraRayEmitter() {
    /* void */
}

manta::StandardCameraRayEmitter::~StandardCameraRayEmitter() {
    /* void */
}

void manta::StandardCameraRayEmitter::generateRay(LightRay *ray) const {
    math::Vector2 samplePoint = m_sampler->generate2d();

    math::Vector target = transformToImagePlane(
        math::Vector2(samplePoint.x - (math::real)0.5, samplePoint.y - (math::real)0.5));
    math::Vector dir = math::sub(target, m_position);
    dir = math::normalize(dir);

    ray->setDirection(dir);
    ray->setSource(m_position);
    ray->setIntensity(math::constants::Zero);
    ray->setWeight(math::constants::One);
    ray->setImagePlaneLocation(
        math::Vector2(
            samplePoint.x - (math::real)0.5 + (math::real)m_pixelX,
            -(samplePoint.y - (math::real)0.5) + (math::real)m_pixelY));
}
