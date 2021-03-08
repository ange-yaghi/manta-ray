#include "../include/lens_camera_ray_emitter_group.h"

#include "../include/lens_camera_ray_emitter.h"
#include "../include/lens.h"

manta::LensCameraRayEmitterGroup::LensCameraRayEmitterGroup() {
    m_lens = nullptr;
}

manta::LensCameraRayEmitterGroup::~LensCameraRayEmitterGroup() {
    /* void */
}

void manta::LensCameraRayEmitterGroup::configure() {
    const int resolutionX = m_lens->getSensorResolutionX();
    const int resolutionY = m_lens->getSensorResolutionY();

    const int nRays = resolutionX * resolutionY;

    m_xIncrement = m_lens->getSensorWidth() / getResolutionX();
    m_yIncrement = m_lens->getSensorHeight() / getResolutionY();

    setUp(m_lens->getUp());
    setDirection(m_lens->getDirection());
    setPosition(m_lens->getPosition());
    setResolutionX(m_lens->getSensorResolutionX());
    setResolutionY(m_lens->getSensorResolutionY());
}

manta::CameraRayEmitter *manta::LensCameraRayEmitterGroup::
    createEmitter(int ix, int iy, StackAllocator *stackAllocator) const 
{
    LensCameraRayEmitter *newEmitter = allocateEmitter<LensCameraRayEmitter>(stackAllocator);

    const math::real x = (ix + (math::real)0.5) * m_xIncrement;
    const math::real y = (iy + (math::real)0.5) * m_yIncrement;

    const math::Vector offset_x = math::loadScalar(-x + m_lens->getSensorWidth() / (math::real)2.0);
    const math::Vector offset_y = math::loadScalar(y - m_lens->getSensorHeight() / (math::real)2.0);

    math::Vector loc = m_lens->getSensorLocation();
    loc = math::add(loc, math::mul(m_lens->getUp(), offset_y));
    loc = math::add(loc, math::mul(m_lens->getSensorRight(), offset_x));

    newEmitter->setPosition(m_position);
    newEmitter->setImagePlaneOrigin(loc);
    newEmitter->setSampler(m_sampler);
    newEmitter->setLens(m_lens);
    newEmitter->setRight(m_lens->getSensorRight());
    newEmitter->setUp(m_lens->getSensorUp());
    newEmitter->setPixelIncrement(math::Vector2(m_xIncrement, m_yIncrement));
    newEmitter->setImagePlaneCoordinates(ix, iy);

    return (CameraRayEmitter *)newEmitter;
}

void manta::LensCameraRayEmitterGroup::_initialize() {
    CameraRayEmitterGroup::_initialize();
}

void manta::LensCameraRayEmitterGroup::_evaluate() {
    Lens *lens = getObject<Lens>(m_lensInput);

    setLens(lens);
    setOutput(this);

    configure();
}

void manta::LensCameraRayEmitterGroup::_destroy() {
    /* void */
}

void manta::LensCameraRayEmitterGroup::registerInputs() {
    registerInput(&m_lensInput, "lens");
}
