#include "../include/complex_map_2d_node.h"

#include "../include/complex_node_output.h"
#include "../include/intersection_point.h"

manta::ComplexMap2dNode::ComplexMap2dNode() {
    m_width = nullptr;
    m_height = nullptr;
}

manta::ComplexMap2dNode::~ComplexMap2dNode() {
    /* void */
}

void manta::ComplexMap2dNode::_initialize() {
    /* void */
}

void manta::ComplexMap2dNode::_evaluate() {
    piranha::native_int width, height;
    m_width->fullCompute(&width);
    m_height->fullCompute(&height);

    if (width > 0 && height > 0) {
        m_map.initialize(width, height);

        ComplexNodeOutput *input = static_cast<ComplexNodeOutput *>(m_input);
        IntersectionPoint p{};
        math::Complex last;
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                math::Complex value;
                p.m_textureCoodinates = math::loadVector(
                    i / (math::real)width,
                    j / (math::real)height);
                input->sample(&p, &value);

                m_map.set(value, i, j);

                last = value;
            }
        }
    }

    m_output.setReference(&m_map);
}

void manta::ComplexMap2dNode::_destroy() {
    m_map.destroy();
}

void manta::ComplexMap2dNode::registerInputs() {
    registerInput(&m_width, "width");
    registerInput(&m_height, "height");
    registerInput(&m_input, "value");
}

void manta::ComplexMap2dNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}
