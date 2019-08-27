#include "../include/lens_camera_ray_emitter.h"

#include "../include/light_ray.h"
#include "../include/lens.h"
#include "../include/sampler_2d.h"
#include "../include/stack_allocator.h"
#include "../include/ray_container.h"

manta::LensCameraRayEmitter::LensCameraRayEmitter() {
    m_lens = nullptr;
}

manta::LensCameraRayEmitter::~LensCameraRayEmitter() {
    /* void */
}

void manta::LensCameraRayEmitter::generateRays(RayContainer *rayContainer) const {
    Lens::LensScanHint hint;
    m_lens->lensScan(m_imagePlaneOrigin, 4, m_pixelIncrement.x, &hint);
    if (hint.failed) return;

    int totalRayCount = m_sampler->getTotalSampleCount(m_sampleCount);
    
    // Create all rays
    rayContainer->initializeRays(totalRayCount);
    rayContainer->setDegree(0);
    LightRay *rays = rayContainer->getRays();

    math::Vector2 *sampleOrigins = 
        (math::Vector2 *)m_stackAllocator->allocate(sizeof(math::Vector2) * totalRayCount, 1);
    m_sampler->generateSamples(totalRayCount, sampleOrigins);

    for (int i = 0; i < totalRayCount; i++) {
        LightRay &ray = rays[i];

        const math::Vector2 samplePoint = sampleOrigins[i];
        math::Vector position = transformToImagePlane(
            math::Vector2(samplePoint.x - (math::real)0.5, samplePoint.y - (math::real)0.5));

        ray.setIntensity(math::constants::Zero);
        ray.setWeight(math::constants::One);

        bool result = m_lens->generateOutgoingRay(position, &hint, &rays[i]);

        ray.setImagePlaneLocation(
            math::Vector2(
                -(samplePoint.x - (math::real)0.5) + (math::real)m_pixelX,
                (samplePoint.y - (math::real)0.5) + (math::real)m_pixelY));
    }

    m_stackAllocator->free((void *)sampleOrigins);
}
