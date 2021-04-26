#include "../include/aces_fitted_node_output.h"

#include "../include/rgb_space.h"

manta::ACESFittedNodeOutput::ACESFittedNodeOutput() {
    m_color = nullptr;
}

manta::ACESFittedNodeOutput::~ACESFittedNodeOutput() {
    /* void */
}

void manta::ACESFittedNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target) const {
    math::Vector input, *output = reinterpret_cast<math::Vector *>(target);
    static_cast<VectorNodeOutput *>(m_color)->sample(surfaceInteraction, &input);

    *output = HillACESFitted(input);
}

void manta::ACESFittedNodeOutput::discreteSample2d(int x, int y, void *target) const {
    math::Vector input, *output = reinterpret_cast<math::Vector *>(target);
    static_cast<VectorNodeOutput *>(m_color)->discreteSample2d(x, y, &input);

    *output = HillACESFitted(input);
}

void manta::ACESFittedNodeOutput::registerInputs() {
    registerInput(&m_color);
}

void manta::ACESFittedNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_color);
    input->evaluateDimensions();

    setDimensions(input->getDimensions());
    for (int i = 0; i < input->getDimensions(); i++) {
        setDimensionSize(i, input->getSize(i));
    }
}

manta::math::Vector manta::ACESFittedNodeOutput::linearToSrgb(const math::Vector &v) {
    math::real r = math::getX(v);
    math::real g = math::getY(v);
    math::real b = math::getZ(v);
    math::real a = math::getW(v);

    RgbSpace::srgb.applyGammaSrgb(r);
    RgbSpace::srgb.applyGammaSrgb(g);
    RgbSpace::srgb.applyGammaSrgb(b);
    RgbSpace::srgb.applyGammaSrgb(a);

    return math::loadVector(r, g, b, a);
}

manta::math::Vector manta::ACESFittedNodeOutput::srgbToLinear(const math::Vector &v) {
    math::real r = math::getX(v);
    math::real g = math::getY(v);
    math::real b = math::getZ(v);
    math::real a = math::getW(v);

    RgbSpace::srgb.inverseGammaSrgb(r);
    RgbSpace::srgb.inverseGammaSrgb(g);
    RgbSpace::srgb.inverseGammaSrgb(b);
    RgbSpace::srgb.inverseGammaSrgb(a);

    return math::loadVector(r, g, b, a);
}

manta::math::Vector manta::ACESFittedNodeOutput::RRTAndODTFit(const math::Vector &v) {
    const math::Vector a = math::sub(
        math::mul(v, math::add(v, math::loadScalar((math::real)0.0245786))),
        math::loadScalar((math::real)0.000090537));
    const math::Vector b = math::add(
        math::mul(
            v,
            math::add(
                math::mul(v, math::loadScalar((math::real)0.983729)),
                math::loadScalar((math::real)0.4329510))),
        math::loadScalar((math::real)0.238081));
    return math::div(a, b);
}

manta::math::Vector manta::ACESFittedNodeOutput::HillACESFitted(const math::Vector &color) {
    static const math::Matrix ACESInputTransform = math::loadMatrix(
        { (math::real)0.59719, (math::real)0.35458, (math::real)0.04823, (math::real)0.0 },
        { (math::real)0.07600, (math::real)0.90834, (math::real)0.01566, (math::real)0.0 },
        { (math::real)0.02840, (math::real)0.13383, (math::real)0.83777, (math::real)0.0 },
        math::constants::IdentityRow4
    );

    static const math::Matrix ACESOutputTransform = math::loadMatrix(
        { (math::real)1.60475, (math::real)-0.53108, (math::real)-0.07367, (math::real)0.0 },
        { (math::real)-0.10208,  (math::real)1.10813, (math::real)-0.00605, (math::real)0.0 },
        { (math::real)-0.00327, (math::real)-0.07276,  (math::real)1.07602, (math::real)0.0 },
        math::constants::IdentityRow4
    );

    // This fit was developed by Stephen Hill (@self_shadow)
    math::Vector t = linearToSrgb(math::mask(color, math::constants::MaskOffW));
    t = math::matMult(ACESInputTransform, t);
    t = RRTAndODTFit(t);
    t = math::matMult(ACESOutputTransform, t);
    return srgbToLinear(math::clamp(t));
}
