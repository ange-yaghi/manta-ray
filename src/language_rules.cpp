#include "../include/language_rules.h"

#include "../include/vector_split_node.h"
#include "../include/constructed_vector_node.h"
#include "../include/vector_conversions.h"
#include "../include/object_channel_types.h"
#include "../include/phong_distribution.h"

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
    registerBuiltinType<VectorSplitNode>(
        "__mantaray__vector", &VectorNodeOutput::VectorType);
    registerBuiltinType <piranha::NoOpNode> (
        "__mantaray__string", &piranha::FundamentalType::StringType);
    registerBuiltinType <piranha::NoOpNode>(
        "__mantaray__microfacet_distribution", &ObjectChannel::MicrofacetDistributionChannel);

    // Constructors
    registerBuiltinType<ConstructedVectorNode>(
        "__mantaray__vector_constructor");
    registerBuiltinType<PhongDistribution>(
        "__mantaray__phong_distribution");

    // Literals
    registerBuiltinType<piranha::DefaultLiteralFloatNode>(
        "__mantaray__literal_float", &piranha::FundamentalType::FloatType);
    registerBuiltinType<piranha::DefaultLiteralStringNode>(
        "__mantaray__literal_string", &piranha::FundamentalType::StringType);

    // Conversions
    registerBuiltinType<FloatToVectorConversionNode>(
        "__mantaray__float_to_vector");

    // ====================================================
    // Literal types
    // ====================================================
    registerLiteralType(piranha::LITERAL_FLOAT, "__mantaray__literal_float");
    registerLiteralType(piranha::LITERAL_STRING, "__mantaray__literal_string");

    // ====================================================
    // Conversions
    // ====================================================
    registerConversion(
        { &piranha::FundamentalType::FloatType, &VectorNodeOutput::VectorType },
        "__mantaray__float_to_vector"
    );
}
