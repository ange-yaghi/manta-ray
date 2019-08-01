#include <pch.h>

#include "../include/obj_file_loader.h"
#include "../include/mesh.h"
#include "../include/light_ray.h"
#include "../include/octree.h"
#include "../include/scene_object.h"

#include <chrono>
#include <fstream>

using namespace manta;

TEST(OctreeTests, OctreeIntersectionTest) {
    Octree octree;
    octree.initialize(1.0, math::loadVector(100, 100, 100));

    bool test = octree.checkTriangle(&octree.m_tree,
        math::loadVector(0.0, 100.0, 0.0),
        math::loadVector(100.0, 100.0, 100.0),
        math::loadVector(0.0, 100.0, 110.0)
    );
}

TEST(OctreeTests, OctreeTestSanityCheck) {
    /* TODO */
}
