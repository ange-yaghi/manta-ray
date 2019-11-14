#include "../include/language_rules.h"

#include "../include/vector_split_node.h"
#include "../include/constructed_vector_node.h"
#include "../include/vector_conversions.h"
#include "../include/object_channel_types.h"
#include "../include/phong_distribution.h"
#include "../include/ggx_distribution.h"
#include "../include/disney_ggx_distribution.h"
#include "../include/disney_gtr_clearcoat_distribution.h"
#include "../include/bsdf.h"
#include "../include/simple_bsdf_material.h"
#include "../include/bilayer_brdf.h"
#include "../include/disney_diffuse_brdf.h"
#include "../include/disney_specular_brdf.h"
#include "../include/material_library.h"
#include "../include/microfacet_brdf.h"
#include "../include/kd_tree.h"
#include "../include/obj_file_node.h"
#include "../include/scene.h"
#include "../include/scene_object.h"
#include "../include/lambertian_brdf.h"
#include "../include/grid_sampler.h"
#include "../include/random_sampler.h"
#include "../include/simple_sampler.h"
#include "../include/standard_camera_ray_emitter_group.h"
#include "../include/ray_tracer.h"
#include "../include/image_output_node.h"
#include "../include/binary_node.h"
#include "../include/unary_node.h"
#include "../include/mesh_merge_node.h"
#include "../include/sphere_primitive.h"
#include "../include/srgb_node.h"
#include "../include/image_file_node.h"
#include "../include/circular_aperture.h"
#include "../include/polygonal_aperture.h"
#include "../include/square_aperture.h"
#include "../include/fraunhofer_diffraction.h"
#include "../include/convolution_node.h"
#include "../include/step_node.h"
#include "../include/padded_frame_output.h"
#include "../include/current_date_node.h"
#include "../include/date_interface_node.h"
#include "../include/lens.h"
#include "../include/simple_lens.h"
#include "../include/lens_camera_ray_emitter_group.h"
#include "../include/surface_interaction_node.h"
#include "../include/media_interface.h"
#include "../include/opaque_media_interface.h"
#include "../include/dielectric_media_interface.h"
#include "../include/microfacet_glass_bsdf.h"
#include "../include/triangle_filter.h"
#include "../include/box_filter.h"
#include "../include/gaussian_filter.h"
#include "../include/main_script_path.h"
#include "../include/script_path_node.h"
#include "../include/append_path_node.h"
#include "../include/bxdf_to_bsdf_node.h"
#include "../include/add_bxdf_node.h"

manta::LanguageRules::LanguageRules() {
    /* void */
}

manta::LanguageRules::~LanguageRules() {
    /* void */
}

void manta::LanguageRules::registerBuiltinNodeTypes() {
    // ====================================================
    // Builtin types
    // ====================================================

    // Channels
    registerBuiltinType < piranha::ChannelNode> (
        "__mantaray__float", &piranha::FundamentalType::FloatType);
    registerBuiltinType<piranha::ChannelNode>(
        "__mantaray__int", &piranha::FundamentalType::IntType);
    registerBuiltinType<piranha::ChannelNode>(
        "__mantaray__bool", &piranha::FundamentalType::BoolType);
    registerBuiltinType<VectorSplitNode>(
        "__mantaray__vector", &VectorNodeOutput::VectorType);
    registerBuiltinType <piranha::ChannelNode> (
        "__mantaray__string", &piranha::FundamentalType::StringType);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__microfacet_distribution", &ObjectChannel::MicrofacetDistributionChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__material", &ObjectChannel::MaterialChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__material_library_channel", &ObjectChannel::MaterialLibraryChannel);
    registerBuiltinType <BSDF>(
        "__mantaray__bsdf", &ObjectChannel::BsdfChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__bxdf", &ObjectChannel::BxdfChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__mesh", &ObjectChannel::MeshChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__scene_geometry", &ObjectChannel::SceneGeometryChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__scene_object_channel", &ObjectChannel::SceneGeometryChannel);
    registerBuiltinType <Scene>(
        "__mantaray__scene", &ObjectChannel::SceneChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__camera", &ObjectChannel::CameraChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__sampler", &ObjectChannel::SamplerChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__vector_map", &VectorMap2DNodeOutput::VectorMap2dType);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__aperture", &ObjectChannel::ApertureChannel);
    registerBuiltinType <DateInterfaceNode>(
        "__mantaray__date", &DateNodeOutput::DateType);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__lens", &ObjectChannel::LensChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__media_interface", &ObjectChannel::MediaInterfaceChannel);
    registerBuiltinType <piranha::ChannelNode>(
        "__mantaray__filter", &ObjectChannel::FilterChannel);

    // Constructors
    registerBuiltinType<MaterialLibrary>(
        "__mantaray__material_library");
    registerBuiltinType<RayTracer>(
        "__mantaray__ray_tracer");
    registerBuiltinType<ConstructedVectorNode>(
        "__mantaray__vector_constructor");
    registerBuiltinType<KDTree>(
        "__mantaray__kd_tree");
    registerBuiltinType<PhongDistribution>(
        "__mantaray__phong_distribution");
    registerBuiltinType<GgxDistribution>(
        "__mantaray__ggx_distribution");
    registerBuiltinType<DisneyGgxDistribution>(
        "__mantaray__disney_ggx_distribution");
    registerBuiltinType<DisneyGtrClearcoatDistribution>(
        "__mantaray__disney_gtr_clearcoat_distribution");
    registerBuiltinType<NullReferenceNode<BSDF>>(
        "__mantaray__null_bsdf");
    registerBuiltinType<NullReferenceNode<BXDF>>(
        "__mantaray__null_bxdf");
    registerBuiltinType<AddBxdfNode>(
        "__mantaray__add_bxdf");
    registerBuiltinType<MicrofacetBRDF>(
        "__mantaray__microfacet_brdf");
    registerBuiltinType<SimpleBSDFMaterial>(
        "__mantaray__simple_bsdf_material");
    registerBuiltinType<BilayerBRDF>(
        "__mantaray__bilayer_brdf");
    registerBuiltinType<DisneyDiffuseBRDF>(
        "__mantaray__disney_diffuse_brdf");
    registerBuiltinType<DisneySpecularBRDF>(
        "__mantaray__disney_specular_brdf");
    registerBuiltinType<ObjFileNode>(
        "__mantaray__obj_file");
    registerBuiltinType<ObjFileNode>(
        "__mantaray__obj_file");
    registerBuiltinType <SceneObject>(
        "__mantaray__scene_object");
    registerBuiltinType <LambertianBRDF>(
        "__mantaray__lambertian_brdf");
    registerBuiltinType <GridSampler>(
        "__mantaray__grid_sampler");
    registerBuiltinType <RandomSampler>(
        "__mantaray__random_sampler");
    registerBuiltinType <SimpleSampler>(
        "__mantaray__simple_sampler");
    registerBuiltinType <StandardCameraRayEmitterGroup>(
        "__mantaray__standard_camera");
    registerBuiltinType <ImageOutputNode>(
        "__mantaray__image_output");
    registerBuiltinType <SpherePrimitive>(
        "__mantaray__sphere");
    registerBuiltinType <SrgbNode>(
        "__mantaray__srgb");
    registerBuiltinType <ImageFileNode>(
        "__mantaray__image_file");
    registerBuiltinType <CircularAperture>(
        "__mantaray__circular_aperture");
    registerBuiltinType <SquareAperture>(
        "__mantaray__square_aperture");
    registerBuiltinType <PolygonalAperture>(
        "__mantaray__polygonal_aperture");
    registerBuiltinType <FraunhoferDiffraction>(
        "__mantaray__fraunhofer_diffraction");
    registerBuiltinType <ConvolutionNode>(
        "__mantaray__convolve_2d");
    registerBuiltinType <StepNode>(
        "__mantaray__step");
    registerBuiltinType <PaddedFrameNode>(
        "__mantaray__padded_frame");
    registerBuiltinType <CurrentDateNode>(
        "__mantaray__current_date");
    registerBuiltinType <SimpleLens>(
        "__mantaray__simple_lens");
    registerBuiltinType <LensCameraRayEmitterGroup>(
        "__mantaray__lens_camera");
    registerBuiltinType <OpaqueMediaInterface>(
        "__mantaray__opaque_media_interface");
    registerBuiltinType <DielectricMediaInterface>(
        "__mantaray__dielectric_media_interface");
    registerBuiltinType <MicrofacetGlassBSDF>(
        "__mantaray__glass_bsdf");
    registerBuiltinType <TriangleFilter>(
        "__mantaray__triangle_filter");
    registerBuiltinType <BoxFilter>(
        "__mantaray__box_filter");
    registerBuiltinType <GaussianFilter>(
        "__mantaray__gaussian_filter");
    registerBuiltinType <MainScriptPathNode>(
        "__mantaray__main_script_path");
    registerBuiltinType <ScriptPathNode>(
        "__mantaray__script_path");
    registerBuiltinType <AppendPathNode>(
        "__mantaray__append_path");

    // Actions
    registerBuiltinType <piranha::ConsoleInputNode>(
        "__mantaray__console_in");
    registerBuiltinType <piranha::ConsoleOutputNode>(
        "__mantaray__console_out");

    // Literals
    registerBuiltinType<piranha::DefaultLiteralFloatNode>(
        "__mantaray__literal_float", &piranha::FundamentalType::FloatType);
    registerBuiltinType<piranha::DefaultLiteralStringNode>(
        "__mantaray__literal_string", &piranha::FundamentalType::StringType);
    registerBuiltinType<piranha::DefaultLiteralIntNode>(
        "__mantaray__literal_int", &piranha::FundamentalType::IntType);
    registerBuiltinType<piranha::DefaultLiteralBoolNode>(
        "__mantaray__literal_bool", &piranha::FundamentalType::BoolType);

    // Conversions
    registerBuiltinType<FloatToVectorConversionNode>(
        "__mantaray__float_to_vector");
    registerBuiltinType<IntToVectorConversionNode>(
        "__mantaray__int_to_vector");
    registerBuiltinType<piranha::IntToFloatConversionNode>(
        "__mantaray__int_to_float");
    registerBuiltinType<piranha::IntToStringConversionNode>(
        "__mantaray__int_to_string");
    registerBuiltinType<piranha::StringToIntConversionNode>(
        "__mantaray__string_to_int");

    registerBuiltinType<BxdfToBsdfNode>(
        "__mantaray__bxdf_to_bsdf");

    // Unary operations
    registerBuiltinType<piranha::NumNegateOperationNode<piranha::native_float>>(
        "__mantaray__float_negate");
    registerBuiltinType<VectorNegateNode>(
        "__mantaray__vector_negate");
    registerBuiltinType<VectorMagnitudeNode>(
        "__mantaray__vector_magnitude");
    registerBuiltinType<VectorNormalizeNode>(
        "__mantaray__vector_normalize");
    registerBuiltinType<VectorMaxComponentNode>(
        "__mantaray__vector_max_component");
    registerBuiltinType<VectorAbsoluteNode>(
        "__mantaray__vector_absolute");

    registerBuiltinType<UvWrapNode>(
        "__mantaray__uv_wrap");

    // Binary operations
    registerBuiltinType<AddNode>(
        "__mantaray__vector_add");
    registerBuiltinType<SubtractNode>(
        "__mantaray__vector_sub");
    registerBuiltinType<DivideNode>(
        "__mantaray__vector_div");
    registerBuiltinType<MultiplyNode>(
        "__mantaray__vector_mul");
    registerBuiltinType<DotNode>(
        "__mantaray__vector_dot");
    registerBuiltinType<CrossNode>(
        "__mantaray__vector_cross");
    registerBuiltinType<PowerNode>(
        "__mantaray__vector_pow");
    registerBuiltinType<MaxNode>(
        "__mantaray__vector_max");
    registerBuiltinType<MinNode>(
        "__mantaray__vector_min");
    registerBuiltinType<MeshMergeNode>(
        "__mantaray__mesh_merge");
    registerBuiltinType<piranha::OperationNodeSpecialized<
        piranha::native_float, piranha::DivideOperationNodeOutput>>("__mantaray__float_divide");
    registerBuiltinType<piranha::OperationNodeSpecialized<
        piranha::native_string, piranha::AddOperationNodeOutput>>("__mantaray__string_add");

    // Surface interaction
    registerBuiltinType<SurfaceInteractionNode>(
        "__mantaray__surface_interaction");

    // ====================================================
    // Literal types
    // ====================================================
    registerLiteralType(piranha::LITERAL_FLOAT, "__mantaray__literal_float");
    registerLiteralType(piranha::LITERAL_STRING, "__mantaray__literal_string");
    registerLiteralType(piranha::LITERAL_INT, "__mantaray__literal_int");
    registerLiteralType(piranha::LITERAL_BOOL, "__mantaray__literal_bool");

    // ====================================================
    // Conversions
    // ====================================================
    registerConversion(
        { &piranha::FundamentalType::FloatType, &VectorNodeOutput::VectorType },
        "__mantaray__float_to_vector"
    );
    registerConversion(
        { &piranha::FundamentalType::IntType, &VectorNodeOutput::VectorType },
        "__mantaray__int_to_vector"
    );
    registerConversion(
        { &piranha::FundamentalType::IntType, &piranha::FundamentalType::FloatType },
        "__mantaray__int_to_float"
    );
    registerConversion(
        { &piranha::FundamentalType::IntType, &piranha::FundamentalType::StringType },
        "__mantaray__int_to_string"
    );
    registerConversion(
        { &piranha::FundamentalType::StringType, &piranha::FundamentalType::IntType },
        "__mantaray__string_to_int"
    );

    registerConversion(
        { &ObjectChannel::BxdfChannel, &ObjectChannel::BsdfChannel },
        "__mantaray__bxdf_to_bsdf"
    );

    // ====================================================
    // Binary operators
    // ====================================================

    // Vector operators
    registerOperator(
        { piranha::IrBinaryOperator::MUL, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_mul"
    );
    registerOperator(
        { piranha::IrBinaryOperator::MUL, &VectorNodeOutput::VectorType, &piranha::FundamentalType::FloatType },
        "__mantaray__vector_mul"
    );
    registerOperator(
        { piranha::IrBinaryOperator::MUL, &VectorNodeOutput::VectorType, &piranha::FundamentalType::IntType },
        "__mantaray__vector_mul"
    );
    registerOperator(
        { piranha::IrBinaryOperator::DIV, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_div"
    );
    registerOperator(
        { piranha::IrBinaryOperator::DIV, &VectorNodeOutput::VectorType, &piranha::FundamentalType::FloatType },
        "__mantaray__vector_div"
    );
    registerOperator(
        { piranha::IrBinaryOperator::DIV, &VectorNodeOutput::VectorType, &piranha::FundamentalType::IntType },
        "__mantaray__vector_div"
    );
    registerOperator(
        { piranha::IrBinaryOperator::ADD, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_add"
    );
    registerOperator(
        { piranha::IrBinaryOperator::ADD, &VectorNodeOutput::VectorType, &piranha::FundamentalType::FloatType },
        "__mantaray__vector_add"
    );
    registerOperator(
        { piranha::IrBinaryOperator::ADD, &VectorNodeOutput::VectorType, &piranha::FundamentalType::IntType },
        "__mantaray__vector_add"
    );
    registerOperator(
        { piranha::IrBinaryOperator::SUB, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_sub"
    );
    registerOperator(
        { piranha::IrBinaryOperator::SUB, &VectorNodeOutput::VectorType, &piranha::FundamentalType::FloatType },
        "__mantaray__vector_sub"
    );
    registerOperator(
        { piranha::IrBinaryOperator::SUB, &VectorNodeOutput::VectorType, &piranha::FundamentalType::IntType },
        "__mantaray__vector_sub"
    );

    // Float operators
    registerOperator(
        { piranha::IrBinaryOperator::DIV, &piranha::FundamentalType::FloatType, &piranha::FundamentalType::FloatType },
        "__mantaray__float_divide"
    );
    registerOperator(
        { piranha::IrBinaryOperator::DIV, &piranha::FundamentalType::FloatType, &piranha::FundamentalType::IntType },
        "__mantaray__float_divide"
    );

    // String operators
    registerOperator(
        { piranha::IrBinaryOperator::ADD, &piranha::FundamentalType::StringType, &piranha::FundamentalType::StringType },
        "__mantaray__string_add"
    );

    // ====================================================
    // Unary operators
    // ====================================================
    registerUnaryOperator(
        { piranha::IrUnaryOperator::NUM_NEGATE, &piranha::FundamentalType::FloatType },
        "__mantaray__float_negate"
    );
    registerUnaryOperator(
        { piranha::IrUnaryOperator::NUM_NEGATE, &VectorNodeOutput::VectorType },
        "__mantaray__vector_negate"
    );
}
