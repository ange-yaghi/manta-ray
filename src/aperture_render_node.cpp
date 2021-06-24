#include "../include/aperture_render_node.h"

#include "../include/image_plane.h"
#include "../include/aperture.h"

manta::ApertureRenderNode::ApertureRenderNode() {
    m_aperture = nullptr;
    m_imagePlane = nullptr;
    m_outputMap = nullptr;
    m_multisample = nullptr;
    m_scale = nullptr;
}

manta::ApertureRenderNode::~ApertureRenderNode() {
    /* void */
}

void manta::ApertureRenderNode::_initialize() {
    /* void */
}

void manta::ApertureRenderNode::_evaluate() {
    static constexpr int SampleBufferCapacity = 128;

    StackAllocator stack;
    stack.initialize(10 * MB);

    Aperture *aperture = getObject<Aperture>(m_aperture);
    ImagePlane *imagePlane = getObject<ImagePlane>(m_imagePlane);
    const int width = imagePlane->getWidth();
    const int height = imagePlane->getHeight();

    int multisample;
    m_multisample->fullCompute(&multisample);

    piranha::native_float scaleIn;
    m_scale->fullCompute(&scaleIn);

    math::real scale = (math::real)scaleIn;

    ImageSample *samples = new ImageSample[SampleBufferCapacity];
    int sampleCount = 0;

    const math::real radius = aperture->getRadius();

    for (int i = 0; i <= height * multisample; ++i) {
        for (int j = 0; j <= width * multisample; ++j) {
            const math::real u = (math::real)j / (width * multisample);
            const math::real v = (math::real)i / (height * multisample);

            samples[sampleCount++] = {
                math::loadScalar(
                    aperture->isNotBlocked((2.0f * u - 1.0f) * radius / scale, (2.0f * v - 1.0f) * radius / scale)
                        ? aperture->sample(2.0f * u - 1.0f, 2.0f * v - 1.0f)
                        : 0.0f),
                { u * width, v * height }
            };

            if (sampleCount == SampleBufferCapacity) {
                imagePlane->processSamples(samples, sampleCount, &stack);
                sampleCount = 0;
            }

            if (getProgram()->isKilled()) break;
        }

        if (getProgram()->isKilled()) break;
    }

    if (sampleCount > 0) {
        imagePlane->processSamples(samples, sampleCount, &stack);
        sampleCount = 0;
    }

    delete[] samples;

    m_outputMap = new VectorMap2D();
    m_outputMap->copy(imagePlane);

    m_output.setMap(m_outputMap);
}

void manta::ApertureRenderNode::_destroy() {
    if (m_outputMap != nullptr) {
        m_outputMap->destroy();
        delete m_outputMap;
        m_outputMap = nullptr;
    }
}

void manta::ApertureRenderNode::registerInputs() {
    registerInput(&m_aperture, "aperture");
    registerInput(&m_imagePlane, "image_plane");
    registerInput(&m_multisample, "multisample");
    registerInput(&m_scale, "scale");
}

void manta::ApertureRenderNode::registerOutputs() {
    registerOutput(&m_output, "__out");
    setPrimaryOutput("__out");
}
