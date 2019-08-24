#ifndef MANTARAY_DEMOS_DEMOS_H
#define MANTARAY_DEMOS_DEMOS_H

#include "settings.h"
#include "utils.h"

// All required manta-ray includes
#include "../include/manta.h"
#include "../include/light_ray.h"
#include "../include/sphere_primitive.h"
#include "../include/intersection_point.h"
#include "../include/scene.h"
#include "../include/scene_object.h"
#include "../include/scene_geometry.h"
#include "../include/material.h"
#include "../include/ray_tracer.h"
#include "../include/memory_management.h"
#include "../include/mesh.h"
#include "../include/obj_file_loader.h"
#include "../include/manta_math.h"
#include "../include/image_plane.h"
#include "../include/raw_file.h"
#include "../include/octree.h"
#include "../include/lens_camera_ray_emitter.h"
#include "../include/lens_camera_ray_emitter_group.h"
#include "../include/simple_lens.h"
#include "../include/texture_node.h"
#include "../include/standard_allocator.h"
#include "../include/grid_sampler.h"
#include "../include/random_sampler.h"
#include "../include/simple_sampler.h"
#include "../include/standard_camera_ray_emitter_group.h"
#include "../include/kd_tree.h"
#include "../include/media_interface.h"
#include "../include/dielectric_media_interface.h"
#include "../include/simple_bsdf_material.h"
#include "../include/bilayer_bsdf.h"
#include "../include/phong_distribution.h"
#include "../include/lambertian_bsdf.h"
#include "../include/ggx_distribution.h"
#include "../include/microfacet_reflection_bsdf.h"
#include "../include/microfacet_transmission_bsdf.h"
#include "../include/microfacet_glass_bsdf.h"
#include "../include/material_pointer.h"
#include "../include/remap_node.h"
#include "../include/square_aperture.h"
#include "../include/polygonal_aperture.h"
#include "../include/circle_convolution.h"
#include "../include/starburst_approximation.h"
#include "../include/fraunhofer_diffraction.h"
#include "../include/image_byte_buffer.h"
#include "../include/jpeg_writer.h"
#include "../include/complex_map_2d.h"
#include "../include/cmf_table.h"
#include "../include/color.h"
#include "../include/spectrum.h"
#include "../include/vector_map_wrapper_node.h"
#include "../include/convolution_node.h"
#include "../include/image_output_node.h"
#include "../include/ramp_node.h"
#include "../include/step_node.h"
#include "../include/material_library.h"
#include "../include/obj_file_node.h"
#include "../include/kd_tree_node.h"
#include "../include/cached_vector_node.h"
#include "../include/binary_node.h"

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

#endif /* MANTARAY_DEMOS_DEMOS_H */
