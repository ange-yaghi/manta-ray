#include "../include/language_rules.h"

#include "../include/vector_split_node.h"
#include "../include/constructed_vector_node.h"
#include "../include/vector_conversions.h"
#include "../include/object_channel_types.h"
#include "../include/phong_distribution.h"
#include "../include/ggx_distribution.h"
#include "../include/bsdf.h"
#include "../include/simple_bsdf_material.h"
#include "../include/bilayer_bsdf.h"
#include "../include/material_library.h"
#include "../include/microfacet_reflection_bsdf.h"
#include "../include/kd_tree.h"
#include "../include/obj_file_node.h"
#include "../include/scene.h"
#include "../include/scene_object.h"
#include "../include/lambertian_bsdf.h"
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
    registerBuiltinType<piranha::NoOpNode>(
        "__mantaray__float", &piranha::FundamentalType::FloatType);
    registerBuiltinType<piranha::NoOpNode>(
        "__mantaray__int", &piranha::FundamentalType::IntType);
    registerBuiltinType<piranha::NoOpNode>(
        "__mantaray__bool", &piranha::FundamentalType::BoolType);
    registerBuiltinType<VectorSplitNode>(
        "__mantaray__vector", &VectorNodeOutput::VectorType);
    registerBuiltinType <piranha::NoOpNode> (
        "__mantaray__string", &piranha::FundamentalType::StringType);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__microfacet_distribution", &ObjectChannel::MicrofacetDistributionChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__material", &ObjectChannel::MaterialChannel);
    registerBuiltinType <MaterialLibrary>(
        "__mantaray__material_library", &ObjectChannel::MaterialLibraryChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__bsdf", &ObjectChannel::BsdfChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__mesh", &ObjectChannel::MeshChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__scene_geometry", &ObjectChannel::SceneGeometryChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__scene_object_channel", &ObjectChannel::SceneGeometryChannel);
    registerBuiltinType <Scene>(
        "__mantaray__scene", &ObjectChannel::SceneChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__camera", &ObjectChannel::CameraChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__sampler", &ObjectChannel::SamplerChannel);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__vector_map", &VectorMap2DNodeOutput::VectorMap2dType);

    // Constructors
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
    registerBuiltinType<MicrofacetReflectionBSDF>(
        "__mantaray__microfacet_reflection_bsdf");
    registerBuiltinType<SimpleBSDFMaterial>(
        "__mantaray__simple_bsdf_material");
    registerBuiltinType<BilayerBSDF>(
        "__mantaray__bilayer_bsdf");
    registerBuiltinType<ObjFileNode>(
        "__mantaray__obj_file");
    registerBuiltinType<ObjFileNode>(
        "__mantaray__obj_file");
    registerBuiltinType <SceneObject>(
        "__mantaray__scene_object");
    registerBuiltinType <LambertianBSDF>(
        "__mantaray__lambertian_bsdf");
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
    registerBuiltinType<piranha::IntToFloatConversionNode>(
        "__mantaray__int_to_float");

    // Unary operations
    registerBuiltinType<piranha::NumNegateOperationNode<piranha::native_float>>(
        "__mantaray__float_negate");
    registerBuiltinType<NegateNode>(
        "__mantaray__vector_negate");
    registerBuiltinType<MagnitudeNode>(
        "__mantaray__vector_magnitude");
    registerBuiltinType<NormalizeNode>(
        "__mantaray__vector_normalize");

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
    registerBuiltinType<MeshMergeNode>(
        "__mantaray__mesh_merge");
    registerBuiltinType<piranha::OperationNodeSpecialized<
        piranha::native_float, piranha::DivideOperationNodeOutput>>("__mantaray__float_divide");

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
        { &piranha::FundamentalType::IntType, &piranha::FundamentalType::FloatType },
        "__mantaray__int_to_float"
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
        { piranha::IrBinaryOperator::DIV, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_div"
    );
    registerOperator(
        { piranha::IrBinaryOperator::ADD, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
        "__mantaray__vector_add"
    );
    registerOperator(
        { piranha::IrBinaryOperator::SUB, &VectorNodeOutput::VectorType, &VectorNodeOutput::VectorType },
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