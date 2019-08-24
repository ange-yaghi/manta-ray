#include "../include/box_filter.h"

manta::BoxFilter::BoxFilter() {
    /* void */
}

manta::BoxFilter::~BoxFilter() {
    /* void */
}

manta::math::Vector manta::BoxFilter::evaluate(const math::Vector2 &p) {
    return math::constants::One;
}
