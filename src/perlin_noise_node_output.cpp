#include "../include/perlin_noise_node_output.h"

#include <algorithm>
#include <cmath>
#include <math.h>

const int manta::PerlinNoiseNodeOutput::NoisePermutation[] = 
                    { 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36,
                      103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0,
                      26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56,
                      87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
                      77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55,
                      46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
                      187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109,
                      198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126,
                      255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183,
                      170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
                      172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112,
                      104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
                      241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106,
                      157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205,
                      93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
                      151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36,
                      103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0,
                      26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56,
                      87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
                      77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55,
                      46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132,
                      187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109,
                      198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126,
                      255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183,
                      170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
                      172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112,
                      104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
                      241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199, 106,
                      157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236, 205,
                      93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };

manta::PerlinNoiseNodeOutput::PerlinNoiseNodeOutput() {
    m_input = nullptr;
}

manta::PerlinNoiseNodeOutput::~PerlinNoiseNodeOutput() {
    /* void */
}

void manta::PerlinNoiseNodeOutput::sample(const IntersectionPoint *surfaceInteraction, void *target_) const {
    math::Vector input;
    static_cast<VectorNodeOutput *>(m_input)->sample(surfaceInteraction, &input);

    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    *target = noise(input);
}

void manta::PerlinNoiseNodeOutput::discreteSample2D(int x, int y, void *target_) const {
    math::Vector input;
    static_cast<VectorNodeOutput *>(m_input)->discreteSample2d(x, y, &input);

    math::Vector *target = reinterpret_cast<math::Vector *>(target_);
    *target = noise(input);
}

manta::math::real manta::PerlinNoiseNodeOutput::lerp(math::real s, math::real s0, math::real s1) {
    return s * s1 + (1 - s) * s0;
}

manta::math::real manta::PerlinNoiseNodeOutput::smoothStep(math::real min, math::real max, math::real value) {
    const math::real v = clamp((value - min) / (max - min), 0, 1);
    return v * v * (-2 * v + 3);
}

manta::math::real manta::PerlinNoiseNodeOutput::clamp(math::real s, math::real min, math::real max) {
    if (s < min) return min;
    else if (s > max) return max;
    else return s;
}

manta::math::real manta::PerlinNoiseNodeOutput::noiseWeight(math::real t) {
    const math::real t3 = t * t * t;
    const math::real t4 = t3 * t;
    return 6 * t4 * t - 15 * t4 + 10 * t3;
}

manta::math::Vector manta::PerlinNoiseNodeOutput::noise(const math::Vector &coordinates) {
    const math::real x = math::getX(coordinates);
    const math::real y = math::getY(coordinates);
    const math::real z = math::getZ(coordinates);

    int ix = (int)std::floor(x);
    int iy = (int)std::floor(y);
    int iz = (int)std::floor(z);

    const math::real dx = x - ix;
    const math::real dy = y - iy;
    const math::real dz = z - iz;

    ix &= NoisePermutationMapSize - 1;
    iy &= NoisePermutationMapSize - 1;
    iz &= NoisePermutationMapSize - 1;

    const math::real w000 = grad(ix,        iy,     iz,     dx,     dy,     dz);
    const math::real w100 = grad(ix + 1,    iy,     iz,     dx - 1, dy,     dz);
    const math::real w010 = grad(ix,        iy + 1, iz,     dx,     dy - 1, dz);
    const math::real w110 = grad(ix + 1,    iy + 1, iz,     dx - 1, dy - 1, dz);
    const math::real w001 = grad(ix,        iy,     iz + 1, dx,     dy,     dz - 1);
    const math::real w101 = grad(ix + 1,    iy,     iz + 1, dx - 1, dy,     dz - 1);
    const math::real w011 = grad(ix,        iy + 1, iz + 1, dx,     dy - 1, dz - 1);
    const math::real w111 = grad(ix + 1,    iy + 1, iz + 1, dx - 1, dy - 1, dz - 1);

    const math::real wx = noiseWeight(dx);
    const math::real wy = noiseWeight(dy);
    const math::real wz = noiseWeight(dz);

    const math::real x00 = lerp(wx, w000, w100);
    const math::real x10 = lerp(wx, w010, w110);
    const math::real x01 = lerp(wx, w001, w101);
    const math::real x11 = lerp(wx, w011, w111);
    const math::real y0 = lerp(wy, x00, x10);
    const math::real y1 = lerp(wy, x01, x11);

    return math::loadScalar(lerp(wz, y0, y1));
}

manta::math::real manta::PerlinNoiseNodeOutput::grad(int x, int y, int z, math::real dx, math::real dy, math::real dz) {
    const int h = NoisePermutation[NoisePermutation[NoisePermutation[x] + y] + z] & 15;

    const math::real u = h < 8 || h == 12 || h == 13 ? dx : dy;
    const math::real v = h < 4 || h == 12 || h == 13 ? dy : dz;
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
}
