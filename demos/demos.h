#ifndef DEMOS_H
#define DEMOS_H

#include <settings.h>

// All required manta-ray includes
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
#include <ss_camera_ray_emitter_group.h>
#include <ss_camera_ray_emitter.h>
#include <image_handling.h>
#include <memory_management.h>
#include <mesh.h>
#include <obj_file_loader.h>
#include <manta_math.h>
#include <utils.h>
#include <scene_buffer.h>
#include <raw_file.h>
#include <octree.h>

namespace manta_demo {

	void simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void cubeTestDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY);

} /* namespace manta_demo */

#endif /* DEMOS_H */
