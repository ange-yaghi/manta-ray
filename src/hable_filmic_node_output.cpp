#include "../include/hable_filmic_node_output.h"

manta::HableFilmicNodeOutput::HableFilmicNodeOutput() {
    m_color = nullptr;
    m_exposureBias = nullptr;
    m_whitePoint = nullptr;
}

manta::HableFilmicNodeOutput::~HableFilmicNodeOutput() {
    /* void */
}

void manta::HableFilmicNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target) const {
    math::Vector input, *output = reinterpret_cast<math::Vector *>(target);
    static_cast<VectorNodeOutput *>(m_color)->sample(surfaceInteraction, &input);

    *output = hableFilmic(input);
}

void manta::HableFilmicNodeOutput::discreteSample2d(int x, int y, void *target) const {
    math::Vector input, *output = reinterpret_cast<math::Vector *>(target);
    static_cast<VectorNodeOutput *>(m_color)->discreteSample2d(x, y, &input);

    *output = hableFilmic(input);
}

void manta::HableFilmicNodeOutput::registerInputs() {
    registerInput(&m_color);
    registerInput(&m_exposureBias);
    registerInput(&m_whitePoint);
}

void manta::HableFilmicNodeOutput::_evaluateDimensions() {
    VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_color);
    input->evaluateDimensions();

    setDimensions(input->getDimensions());
    for (int i = 0; i < input->getDimensions(); i++) {
        setDimensionSize(i, input->getSize(i));
    }
}

manta::math::Vector manta::HableFilmicNodeOutput::tonemapPartial(const math::Vector &v) {
    static const math::Vector A = math::loadScalar((math::real)0.15);
    static const math::Vector B = math::loadScalar((math::real)0.50);
    static const math::Vector C = math::loadScalar((math::real)0.10);
    static const math::Vector D = math::loadScalar((math::real)0.20);
    static const math::Vector E = math::loadScalar((math::real)0.02);
    static const math::Vector F = math::loadScalar((math::real)0.30);

    // A * v
    const math::Vector A_v = math::mul(A, v);
    
    // A * v + C * B
    const math::Vector s0 = math::add(A_v, math::mul(C, B));

    // x * (A * v + C * B) + D * E
    const math::Vector s1 = math::add(math::mul(v, s0), math::mul(D, E));

    // A * v + B
    const math::Vector s2 = math::add(A_v, B);

    // A * (A * v + B) + D * F
    const math::Vector s3 = math::add(math::mul(v, s2), math::mul(D, F));

    return math::sub(
        math::div(s1, s3),
        math::div(E, F)
    );
}

manta::math::Vector manta::HableFilmicNodeOutput::hableFilmic(const math::Vector &color) const {
    math::Vector exposureBias;
    math::Vector whitePoint;
        
    static_cast<VectorNodeOutput *>(m_exposureBias)->sample(nullptr, &exposureBias);
    static_cast<VectorNodeOutput *>(m_whitePoint)->sample(nullptr, &whitePoint);

    return math::div(
        tonemapPartial(math::mul(exposureBias, color)),
        tonemapPartial(whitePoint));
}
