#ifndef DEMOS_H
#define DEMOS_H

#include <settings.h>

// All required manta-ray includes
#include <manta.h>
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
#include <image_plane.h>
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
#include <square_aperture.h>
#include <polygonal_aperture.h>
#include <circle_convolution.h>
#include <starburst_approximation.h>
#include <fraunhofer_diffraction.h>
#include <image_byte_buffer.h>
#include <jpeg_writer.h>
#include <complex_map_2d.h>
#include <cmf_table.h>
#include <color.h>
#include <spectrum.h>
#include <vector_map_wrapper_node.h>
#include <convolution_node.h>
#include <image_output_node.h>
#include <multiply_node.h>
#include <ramp_node.h>
#include <step_node.h>
#include <material_manager.h>
#include <obj_file_node.h>

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
		STOCK_SCENE,
		APERTURE_DEMO,
		SDL_DEMO
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
	void apertureDemo(int samplesPerPixe, int resolutionX, int resolutionY);
	void sdlDemo(int samplesPerPixe, int resolutionX, int resolutionY);

} /* namespace manta_demo */

#endif /* DEMOS_H */
