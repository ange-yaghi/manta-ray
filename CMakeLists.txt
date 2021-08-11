cmake_minimum_required(VERSION 3.10)

# Enable group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

project(mantaray)

set(CMAKE_CXX_STANDARD 11)

# =========================================================
# libjpeg-turbo

find_package(libjpeg-turbo CONFIG REQUIRED)

# =========================================================
# SDL2

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# =========================================================
# GTEST

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set_property(TARGET gmock PROPERTY FOLDER "gtest")
set_property(TARGET gmock_main PROPERTY FOLDER "gtest")
set_property(TARGET gtest PROPERTY FOLDER "gtest")
set_property(TARGET gtest_main PROPERTY FOLDER "gtest")

include_directories(dependencies/submodules/piranha/include)
include_directories(dependencies/submodules/delta-studio)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.40 COMPONENTS filesystem REQUIRED)

add_library(mantaray STATIC
    # Source files
    src/aces_fitted_node.cpp
    src/aces_fitted_node_output.cpp
    src/add_bxdf_node.cpp
    src/aperture.cpp
    src/aperture_render_node.cpp
    src/append_path_node.cpp
    src/area_light.cpp
    src/biconvex_lens.cpp
    src/bilayer_brdf.cpp
    src/box_filter.cpp
    src/bsdf.cpp
    src/bump_node.cpp
    src/bump_node_output.cpp
    src/bxdf.cpp
    src/bxdf_to_bsdf_node.cpp
    src/cached_vector_node.cpp
    src/cached_vector_output.cpp
    src/camera_ray_emitter.cpp
    src/camera_ray_emitter_group.cpp
    src/cft_estimator_2d.cpp
    src/circle_convolution.cpp
    src/circular_aperture.cpp
    src/cmf_table.cpp
    src/complex_map_2d.cpp
    src/complex_map_2d_node.cpp
    src/complex_map_sampler.cpp
    src/complex_map_sample_node_output.cpp
    src/complex_node_output.cpp
    src/complex_split_node.cpp
    src/complex_split_node_output.cpp
    src/console.cpp
    src/constructed_complex_node.cpp
    src/constructed_complex_node_output.cpp
    src/constructed_vector_node.cpp
    src/constructed_vector_node_output.cpp
    src/convolution.cpp
    src/convolution_node.cpp
    src/current_date_node.cpp
    src/date_interface_node.cpp
    src/date_node_output.cpp
    src/dielectric_media_interface.cpp
    src/disney_diffuse_brdf.cpp
    src/disney_ggx_distribution.cpp
    src/disney_gtr_clearcoat_distribution.cpp
    src/disney_specular_brdf.cpp
    src/filter.cpp
    src/fraunhofer_diffraction.cpp
    src/fraunhofer_diffraction_node.cpp
    src/fresnel_node.cpp
    src/fresnel_node_output.cpp
    src/gaussian_filter.cpp
    src/ggx_distribution.cpp
    src/gpu_kernel_opencl.cpp
    src/gpu_manager_opencl.cpp
    src/gpu_memory_opencl.cpp
    src/hable_filmic_node.cpp
    src/hable_filmic_node_output.cpp
    src/image_byte_buffer.cpp
    src/image_file_node.cpp
    src/image_handling.cpp
    src/image_output_node.cpp
    src/image_plane.cpp
    src/image_plane_converter_node.cpp
    src/intersection_point.cpp
    src/intersection_point_manager.cpp
    src/job_queue.cpp
    src/jpeg_writer.cpp
    src/kd_tree.cpp
    src/kd_tree_node.cpp
    src/lambertian_brdf.cpp
    src/language_rules.cpp
    src/lens.cpp
    src/lens_camera_ray_emitter.cpp
    src/lens_camera_ray_emitter_group.cpp
    src/lens_element.cpp
    src/light.cpp
    src/light_ray.cpp
    src/main_script_path.cpp
    src/manta_math.cpp
    src/manta_math_float_simd.cpp
    src/manta_math_single.cpp
    src/material.cpp
    src/material_library.cpp
    src/material_pointer.cpp
    src/media_interface.cpp
    src/mesh.cpp
    src/mesh_merge_node.cpp
    src/microfacet_brdf.cpp
    src/microfacet_btdf.cpp
    src/microfacet_distribution.cpp
    src/microfacet_glass_bsdf.cpp
    src/node.cpp
    src/object_channel_types.cpp
    src/obj_file_loader.cpp
    src/obj_file_node.cpp
    src/octree.cpp
    src/opaque_media_interface.cpp
    src/os_utilities.cpp
    src/path.cpp
    src/path_recorder.cpp
    src/perfect_specular_reflection_brdf.cpp
    src/perlin_noise_node.cpp
    src/perlin_noise_node_output.cpp
    src/phong_distribution.cpp
    src/pixel_based_sampler.cpp
    src/polygonal_aperture.cpp
    src/preview_node.cpp
    src/primitives.cpp
    src/progressive_resolution_render_pattern.cpp
    src/radial_render_pattern.cpp
    src/ramp_node.cpp
    src/ramp_node_output.cpp
    src/random_render_pattern.cpp
    src/random_sampler.cpp
    src/raw_file.cpp
    src/ray_tracer.cpp
    src/remap_node.cpp
    src/remap_node_output.cpp
    src/render_pattern.cpp
    src/rgb_space.cpp
    src/sampler.cpp
    src/scene.cpp
    src/scene_geometry.cpp
    src/scene_object.cpp
    src/script_path_node.cpp
    src/session.cpp
    src/signal_processing.cpp
    src/simple_bsdf_material.cpp
    src/simple_lens.cpp
    src/spectrum.cpp
    src/specular_glass_bsdf.cpp
    src/sphere_primitive.cpp
    src/spherical_surface.cpp
    src/spiral_render_pattern.cpp
    src/square_aperture.cpp
    src/srgb_node.cpp
    src/srgb_node_output.cpp
    src/stack_allocator.cpp
    src/standard_allocator.cpp
    src/standard_camera_ray_emitter.cpp
    src/standard_camera_ray_emitter_group.cpp
    src/starburst_approximation.cpp
    src/step_node.cpp
    src/step_node_output.cpp
    src/stratified_sampler.cpp
    src/streaming_node_output.cpp
    src/string_conversions.cpp
    src/surface_interaction_node.cpp
    src/texture_node.cpp
    src/triangle_filter.cpp
    src/turbulence_noise_node.cpp
    src/turbulence_noise_node_output.cpp
    src/vector_conversions.cpp
    src/vector_map_2d.cpp
    src/vector_map_2d_node_output.cpp
    src/vector_node_output.cpp
    src/vector_split_node.cpp
    src/vector_split_node_output.cpp
    src/worker.cpp

    # Include files
    include/aces_fitted_node.h
    include/aces_fitted_node_output.h
    include/add_bxdf_node.h
    include/aperture.h
    include/aperture_render_node.h
    include/append_path_node.h
    include/area_light.h
    include/average_node_output.h
    include/biconvex_lens.h
    include/bilayer_brdf.h
    include/binary_node.h
    include/binary_node_output.h
    include/box_filter.h
    include/bsdf.h
    include/bump_node.h
    include/bump_node_output.h
    include/bxdf.h
    include/bxdf_to_bsdf_node.h
    include/cacheable_input.h
    include/cached_vector_node.h
    include/cached_vector_output.h
    include/camera_ray_emitter.h
    include/camera_ray_emitter_group.h
    include/cft_estimator_2d.h
    include/circle_convolution.h
    include/circular_aperture.h
    include/cmf_table.h
    include/coarse_intersection.h
    include/color.h
    include/complex_bake_node.h
    include/complex_map_2d.h
    include/complex_map_2d_node.h
    include/complex_map_2d_node_output.h
    include/complex_map_interface_node.h
    include/complex_map_interface_node_output.h
    include/complex_map_linear_sampler.h
    include/complex_map_linear_sampler_node.h
    include/complex_map_nearest_sampler.h
    include/complex_map_nearest_sampler_node.h
    include/complex_map_operation_node.h
    include/complex_map_sampler.h
    include/complex_map_sample_node.h
    include/complex_map_sample_node_output.h
    include/complex_math.h
    include/complex_node_output.h
    include/complex_split_node.h
    include/complex_split_node_output.h
    include/console.h
    include/console_log_node.h
    include/constructed_complex_node.h
    include/constructed_complex_node_output.h
    include/constructed_vector_node.h
    include/constructed_vector_node_output.h
    include/convolution.h
    include/convolution_node.h
    include/current_date_node.h
    include/date_interface_node.h
    include/date_node_output.h
    include/dielectric_media_interface.h
    include/disney_diffuse_brdf.h
    include/disney_ggx_distribution.h
    include/disney_gtr_clearcoat_distribution.h
    include/disney_specular_brdf.h
    include/filter.h
    include/fraunhofer_diffraction.h
    include/fraunhofer_diffraction_node.h
    include/fresnel_node.h
    include/fresnel_node_output.h
    include/gaussian_filter.h
    include/geometry.h
    include/ggx_distribution.h
    include/gpu_kernel.h
    include/gpu_kernel_opencl.h
    include/gpu_manager.h
    include/gpu_manager_opencl.h
    include/gpu_memory.h
    include/gpu_memory_opencl.h
    include/hable_filmic_node.h
    include/hable_filmic_node_output.h
    include/image_byte_buffer.h
    include/image_file_node.h
    include/image_handling.h
    include/image_output_node.h
    include/image_plane.h
    include/image_plane_converter_node.h
    include/image_sample.h
    include/intersection_point.h
    include/intersection_point_manager.h
    include/intersection_point_types.h
    include/job_queue.h
    include/jpeg_writer.h
    include/kd_tree.h
    include/kd_tree_node.h
    include/lambertian_brdf.h
    include/language_rules.h
    include/lens.h
    include/lens_camera_ray_emitter.h
    include/lens_camera_ray_emitter_group.h
    include/lens_element.h
    include/light.h
    include/light_ray.h
    include/main_script_path.h
    include/manta.h
    include/manta_build_conf.h
    include/manta_math.h
    include/manta_math_conf.h
    include/manta_math_float_simd.h
    include/manta_math_single.h
    include/manta_real.h
    include/margins.h
    include/material.h
    include/material_library.h
    include/material_pointer.h
    include/media_interface.h
    include/memory_management.h
    include/mesh.h
    include/mesh_merge_node.h
    include/microfacet_brdf.h
    include/microfacet_btdf.h
    include/microfacet_distribution.h
    include/microfacet_glass_bsdf.h
    include/mipmap.h
    include/node.h
    include/node_cache.h
    include/object_channel_types.h
    include/object_reference_node.h
    include/object_reference_node_output.h
    include/obj_file_loader.h
    include/obj_file_node.h
    include/octree.h
    include/opaque_media_interface.h
    include/os_utilities.h
    include/padded_frame_output.h
    include/path.h
    include/path_recorder.h
    include/perfect_specular_reflection_brdf.h
    include/perlin_noise_node.h
    include/perlin_noise_node_output.h
    include/phong_distribution.h
    include/pixel_based_sampler.h
    include/polygonal_aperture.h
    include/preview_manager.h
    include/preview_node.h
    include/primitives.h
    include/progressive_resolution_render_pattern.h
    include/radial_render_pattern.h
    include/ramp_node.h
    include/ramp_node_output.h
    include/random_render_pattern.h
    include/random_sampler.h
    include/raw_file.h
    include/ray_tracer.h
    include/remap_node.h
    include/remap_node_output.h
    include/render_pattern.h
    include/rgb_space.h
    include/runtime_statistics.h
    include/sampler.h
    include/scalar_map_2d.h
    include/scene.h
    include/scene_geometry.h
    include/scene_object.h
    include/script_path_node.h
    include/session.h
    include/signal_processing.h
    include/simple_bsdf_material.h
    include/simple_lens.h
    include/spectrum.h
    include/specular_glass_bsdf.h
    include/sphere_primitive.h
    include/spherical_surface.h
    include/spiral_render_pattern.h
    include/square_aperture.h
    include/srgb_node.h
    include/srgb_node_output.h
    include/stack_allocator.h
    include/stack_list.h
    include/standard_allocator.h
    include/standard_camera_ray_emitter.h
    include/standard_camera_ray_emitter_group.h
    include/starburst_approximation.h
    include/step_node.h
    include/step_node_output.h
    include/stratified_sampler.h
    include/streaming_node_output.h
    include/string_conversions.h
    include/surface_interaction_node.h
    include/surface_interaction_node_output.h
    include/texture_node.h
    include/triangle_filter.h
    include/turbulence_noise_node.h
    include/turbulence_noise_node_output.h
    include/unary_node.h
    include/unary_node_output.h
    include/uv_operation_output.h
    include/vector_conversions.h
    include/vector_map_2d.h
    include/vector_map_2d_node_output.h
    include/vector_map_wrapper_node.h
    include/vector_node_output.h
    include/vector_split_node.h
    include/vector_split_node_output.h
    include/worker.h
)

target_link_libraries(mantaray
    piranha
    Boost::filesystem
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    libjpeg-turbo::turbojpeg-static
)

add_subdirectory(dependencies)
add_subdirectory(cli)
add_subdirectory(ui)
