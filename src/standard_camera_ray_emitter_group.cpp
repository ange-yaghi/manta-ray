#include "../include/standard_camera_ray_emitter_group.h"

#include "../include/standard_camera_ray_emitter.h"
#include "../include/sampler.h"

manta::StandardCameraRayEmitterGroup::StandardCameraRayEmitterGroup() {
    /* void */
}

manta::StandardCameraRayEmitterGroup::~StandardCameraRayEmitterGroup() {
    /* void */
}

void manta::StandardCameraRayEmitterGroup::configure() {
    m_planeWidth = (m_resolutionX / (math::real)m_resolutionY) * (math::real)m_planeHeight;

    m_xIncrement = m_planeWidth / m_resolutionX;
    m_yIncrement = (math::real)m_planeHeight / m_resolutionY;

    m_right = math::cross(m_direction, m_up);
    m_planeCenter = math::add(m_position, math::mul(m_direction, math::loadScalar((math::real)m_planeDistance)));

    //m_sampler->setAxis1(m_right);
    //m_sampler->setAxis2(m_up);
    //m_sampler->setBoundaryHeight(m_yIncrement);
    //m_sampler->setBoundaryWidth(m_xIncrement);
}

manta::CameraRayEmitter *manta::StandardCameraRayEmitterGroup::
    createEmitter(int ix, int iy, StackAllocator *stackAllocator) const 
{
    StandardCameraRayEmitter *newEmitter = allocateEmitter<StandardCameraRayEmitter>(stackAllocator);

    math::real x = (ix + (math::real)0.5) * m_xIncrement;
    math::real y = (iy + (math::real)0.5) * m_yIncrement;

    math::Vector loc = math::mul(m_right, math::loadScalar(x - m_planeWidth / (math::real)2.0));
    loc = math::add(loc, math::mul(m_up, math::loadScalar(-y + (math::real)m_planeHeight / (math::real)2.0)));
    loc = math::add(loc, m_planeCenter);

    newEmitter->setPosition(m_position);
    newEmitter->setImagePlaneOrigin(loc);
    newEmitter->setSampler(m_sampler);
    newEmitter->setRight(m_right);
    newEmitter->setUp(m_up);
    newEmitter->setPixelIncrement(math::Vector2(m_xIncrement, m_yIncrement));
    newEmitter->setImagePlaneCoordinates(ix, iy);

    return newEmitter;
}
