#include "../include/standard_camera_ray_emitter.h"

#include "../include/sampler_2d.h"
#include "../include/light_ray.h"
#include "../include/stack_allocator.h"
#include "../include/ray_container.h"

manta::StandardCameraRayEmitter::StandardCameraRayEmitter() {
    /* void */
}

manta::StandardCameraRayEmitter::~StandardCameraRayEmitter() {
    /* void */
}

void manta::StandardCameraRayEmitter::generateRays(RayContainer *rayContainer) const {
    int nRays = m_sampler->getTotalSampleCount(m_sampleCount);
    
    // InitializeRays
    rayContainer->initializeRays(nRays);
    rayContainer->setDegree(0);
    LightRay *rays = rayContainer->getRays();
    
    math::Vector2 *samplePoints = 
        (math::Vector2 *)m_stackAllocator->allocate(nRays * sizeof(math::Vector2));
    m_sampler->generateSamples(nRays, samplePoints);

    for (int i = 0; i < nRays; i++) {
        LightRay *ray = &rays[i];
        const math::Vector2 &samplePoint = samplePoints[i];

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

    m_stackAllocator->free((void *)samplePoints);
}
