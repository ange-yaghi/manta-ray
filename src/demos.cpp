#include <demos.h>

#include <light_ray.h>
#include <sphere_primitive.h>
#include <intersection_point.h>
#include <scene.h>
#include <scene_object.h>
#include <scene_geometry.h>
#include <material.h>
#include <simple_diffuse_material.h>
#include <simple_specular_diffuse_material.h>
#include <ray_tracer.h>
#include <camera_ray_emitter_group.h>
#include <camera_ray_emitter.h>
#include <image_handling.h>
#include <memory_management.h>
#include <mesh.h>
#include <obj_file_loader.h>
#include <manta_math.h>

#include <iostream>

manta::math::Vector getColor(int r, int g, int b) {
	return manta::math::loadVector(r / 255.0f, g / 255.0f, b / 255.0f);
}



