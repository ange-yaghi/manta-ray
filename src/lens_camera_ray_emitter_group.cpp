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

    math::real sensorElementWidth = m_lens->getSensorWidth() / getResolutionX();
    math::real sensorElementHeight = m_lens->getSensorHeight() / getResolutionY();

    m_sampler->setBoundaryWidth(sensorElementWidth);
    m_sampler->setBoundaryHeight(sensorElementHeight);
    m_sampler->setAxis1(m_lens->getSensorRight());
    m_sampler->setAxis2(m_lens->getSensorUp());
}

manta::CameraRayEmitter *manta::LensCameraRayEmitterGroup::createEmitter(int ix, int iy, 
    StackAllocator *stackAllocator) const {
    LensCameraRayEmitter *newEmitter = allocateEmitter<LensCameraRayEmitter>(stackAllocator);

    math::Vector sensorElement = m_lens->getSensorElement(ix, iy);
    newEmitter->setSampleCount(m_samples);
    newEmitter->setPosition(sensorElement);
    newEmitter->setSampler(m_sampler);
    newEmitter->setLens(m_lens);

    return (CameraRayEmitter *)newEmitter;
}

void manta::LensCameraRayEmitterGroup::_initialize() {
    CameraRayEmitterGroup::_initialize();
}

void manta::LensCameraRayEmitterGroup::_evaluate() {
    Lens *lens = getObject<Lens>(m_lensInput);
    m_sampler = getObject<Sampler2D>(m_samplerInput);

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
