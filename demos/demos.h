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
#include <texture_node.h>
#include <standard_allocator.h>
#include <grid_sampler.h>
#include <random_sampler.h>
#include <simple_sampler.h>
#include <standard_camera_ray_emitter_group.h>
#include <kd_tree.h>
#include <media_interface.h>
#include <dielectric_media_interface.h>
#include <simple_bsdf_material.h>
#include <single_color_node.h>
#include <bilayer_bsdf.h>
#include <phong_distribution.h>
#include <lambertian_bsdf.h>
#include <ggx_distribution.h>
#include <microfacet_reflection_bsdf.h>
#include <microfacet_transmission_bsdf.h>
#include <microfacet_glass_bsdf.h>
#include <material_pointer.h>
#include <power_node.h>
#include <remap_node.h>

namespace manta_demo {

	enum DEMO {
		SIMPLE_ROOM,
		TEAPOT_LAMP,
		BOX_CITY,
		BLOCKS,
		STRESS_SPIDERS,
		PEN,
		COMPLEX_ROOM,
		SAMSUNG_A8,
		MATERIAL_PREVIEW,
		STOCK_SCENE
	};

	void runDemo(DEMO demo, int samplesPerPixel, int resolutionX, int resolutionY);

	void simpleRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void teapotLampDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void boxCityDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void blocksDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void stressSpidersDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void penDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void complexRoomDemo(int samplesPerPixel, int resolutionX, int resolutionY);
	void samsungA8Demo(int samplesPerPixel, int resolutionX, int resolutionY);
	void materialVisualizer(int samplesPerPixel, int resolutionX, int resolutionY);
	void stockSceneDemo(int samplesPerPixel, int resolutionX, int resolutionY);

} /* namespace manta_demo */

#endif /* DEMOS_H */
