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
    int totalRayCount = m_sampler->getTotalSampleCount(m_sampleCount);
    
    // Create all rays
    rayContainer->initializeRays(totalRayCount);
    rayContainer->setDegree(0);
    LightRay *rays = rayContainer->getRays();

    math::Vector2 *sampleOrigins = 
        (math::Vector2 *)m_stackAllocator->allocate(sizeof(math::Vector2) * totalRayCount, 1);
    m_sampler->generateSamples(totalRayCount, sampleOrigins);

    Lens::LensScanHint hint;
    m_lens->lensScan(m_position, 4, 0.0 /* TODO: m_sampler->getBoundaryWidth() */, &hint);

    for (int i = 0; i < totalRayCount; i++) {
        const math::Vector2 samplePoint = sampleOrigins[i];
        math::Vector position = transformToImagePlane(samplePoint);

        rays[i].setIntensity(math::constants::Zero);
        rays[i].setWeight(math::constants::One);

        bool result = m_lens->generateOutgoingRay(position, &hint, &rays[i]);
    }

    m_stackAllocator->free((void *)sampleOrigins);
}
