#include "../include/turbulence_noise_node_output.h"

#include "../include/perlin_noise_node_output.h"

manta::TurbulenceNoiseNodeOutput::TurbulenceNoiseNodeOutput() {
    m_input = nullptr;
    m_omega = nullptr;
    m_octaves = nullptr;
}

manta::TurbulenceNoiseNodeOutput::~TurbulenceNoiseNodeOutput() {
    /* void */
}

void manta::TurbulenceNoiseNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target_) const {
    piranha::native_int octaves;
    math::Vector omega;
    math::Vector input;

    m_octaves->fullCompute((void *)&octaves);
    static_cast<VectorNodeOutput *>(m_omega)->sample(surfaceInteraction, &omega);
    static_cast<VectorNodeOutput *>(m_input)->sample(surfaceInteraction, &input);

    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    *target = fractionalBrownianMotion(input, math::getScalar(omega), octaves);
}

void manta::TurbulenceNoiseNodeOutput::discreteSample2d(int x, int y, void *target_) const {
    piranha::native_int octaves;
    math::Vector omega;
    math::Vector input;

    m_octaves->fullCompute((void *)&octaves);
    static_cast<VectorNodeOutput *>(m_omega)->discreteSample2d(x, y, &omega);
    static_cast<VectorNodeOutput *>(m_input)->discreteSample2d(x, y, &input);

    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    *target = fractionalBrownianMotion(input, math::getScalar(omega), octaves);
}

manta::math::Vector manta::TurbulenceNoiseNodeOutput::fractionalBrownianMotion(const math::Vector &input, math::real omega, int octaves) {
    math::Vector sum = math::constants::Zero;
    math::Vector lambda = math::constants::One;
    math::Vector o = math::loadScalar(omega);

    for (int i = 0; i < octaves; ++i) {
        sum = math::add(
            sum,
            math::mul(
                o,
                PerlinNoiseNodeOutput::noise(math::mul(lambda, input))));
        lambda = math::mul(lambda, math::loadScalar((math::real)1.99f));
        o = math::mul(o, math::loadScalar(omega));
    }

    return sum;
}
