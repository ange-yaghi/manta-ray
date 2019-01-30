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
#include <ray_tracer.h>
#include <image_handling.h>
#include <memory_management.h>
#include <mesh.h>
#include <obj_file_loader.h>
#include <manta_math.h>
#include <utils.h>
#include <scene_buffer.h>
#include <raw_file.h>
#include <octree.h>
#include <lens_camera_ray_emitter.h>
#include <lens_camera_ray_emitter_group.h>
#include <simple_lens.h>
#include <texture_map.h>
#include <standard_allocator.h>
#include <phong_phong_bilayer_material.h>
#include <grid_sampler.h>
#include <random_sampler.h>
#include <simple_sampler.h>
#include <standard_camera_ray_emitter_group.h>
#include <kd_tree.h>
#include <media_interface.h>
#include <simple_lambert_material.h>

namespace manta_demo {

	void simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void teapotDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void cubeTestDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void blocksDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void stressSpidersDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void penDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void complexRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY);

} /* namespace manta_demo */

#endif /* DEMOS_H */
