#include "../include/lens_camera_ray_emitter_group.h"

#include "../include/lens_camera_ray_emitter.h"
#include "../include/lens.h"
#include "../include/sampler_2d.h"

manta::LensCameraRayEmitterGroup::LensCameraRayEmitterGroup() {
    m_lens = nullptr;
}

manta::LensCameraRayEmitterGroup::~LensCameraRayEmitterGroup() {
    /* void */
}

void manta::LensCameraRayEmitterGroup::configure() {
    int resolutionX = m_lens->getSensorResolutionX();
    int resolutionY = m_lens->getSensorResolutionY();

    int nRays = resolutionX * resolutionY;

    m_xIncrement = m_lens->getSensorWidth() / getResolutionX();
    m_yIncrement = m_lens->getSensorHeight() / getResolutionY();
}

manta::CameraRayEmitter *manta::LensCameraRayEmitterGroup::
    createEmitter(int ix, int iy, StackAllocator *stackAllocator) const 
{
    LensCameraRayEmitter *newEmitter = allocateEmitter<LensCameraRayEmitter>(stackAllocator);

    math::real x = (ix + (math::real)0.5) * m_xIncrement;
    math::real y = (iy + (math::real)0.5) * m_yIncrement;

    math::Vector loc = math::mul(m_lens->getSensorRight(), math::loadScalar(- x + m_lens->getSensorWidth() / (math::real)2.0));
    loc = math::add(loc, math::mul(m_lens->getUp(), math::loadScalar(y - (math::real)m_lens->getSensorHeight() / (math::real)2.0)));
    loc = math::add(loc, m_lens->getSensorLocation());

    newEmitter->setSampleCount(m_samples);
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
    m_sampler = getObject<Sampler2d>(m_samplerInput);

    piranha::native_int samples;
    m_sampleInput->fullCompute((void *)&samples);

    setUp(lens->getUp());
    setDirection(lens->getDirection());
    setPosition(lens->getPosition());
    setResolutionX(lens->getSensorResolutionX());
    setResolutionY(lens->getSensorResolutionY());
    setLens(lens);
    setSampleCount((int)samples);

    m_output.setReference(this);

    configure();
}

void manta::LensCameraRayEmitterGroup::_destroy() {
    /* void */
}

void manta::LensCameraRayEmitterGroup::registerInputs() {
    registerInput(&m_lensInput, "lens");
    registerInput(&m_sampleInput, "samples");
    registerInput(&m_samplerInput, "sampler");
}
