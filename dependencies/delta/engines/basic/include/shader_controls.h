#ifndef DELTA_BASIC_SHADER_CONTROLS_H
#define DELTA_BASIC_SHADER_CONTROLS_H

#include "delta_core.h"

namespace dbasic {

    struct Vertex {
        ysVector4 Pos = { 0.0f, 0.0f, 0.0f, 0.0f };
        ysVector2 TexCoord = { 0.0f, 0.0f };
        ysVector4 Normal = { 0.0f, 0.0f, 0.0f, 0.0f };
    };

    struct ConsoleVertex {
        ysVector2 Pos = { 0.0f, 0.0f };
        ysVector2 TexCoord = { 0.0f, 0.0f };
        ysVector4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };

    struct ShaderObjectVariables {
        ysMatrix Transform = ysMath::Constants::Identity;

        float TexOffset[2] = { 0.0f, 0.0f };
        float TexScale[2] = { 1.0f, 1.0f };
        float Scale[4] = { 1.0f, 1.0f, 1.0f };

        ysVector4 BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        ysVector4 Emission = { 0.0f, 0.0f, 0.0f, 0.0f };
        float SpecularMix = 1.0f;
        float DiffuseMix = 1.0f;
        float Metallic = 0.0f;
        float DiffuseRoughness = 0.5f;
        float SpecularPower = 4.0f;
        float IncidentSpecular = 1.0f;

        int ColorReplace = 0;
        int Lit = 1;
    };

    struct ConsoleShaderObjectVariables {
        ysVector4 MulCol = { 1.0f, 1.0f, 1.0f, 1.0f };
        float TexOffset[2] = { 0.0f, 0.0f };
        float TexScale[2] = { 1.0f, 1.0f };
    };

    struct ShaderScreenVariables {
        ysMatrix CameraView = ysMath::Constants::Identity;
        ysMatrix Projection = ysMath::Constants::Identity;

        ysVector4 Eye = { 0.0f, 0.0f, 0.0f };

        ysVector4 FogColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        float FogNear = 10.0f;
        float FogFar = 100.0f;

        int Padding[2];
    };

    struct ShaderSkinningControls {
        ysMatrix BoneTransforms[256];
    };

    struct Light {
        ysVector4 Position = { 0.0f, 0.0f, 0.0f, 1.0f };
        ysVector4 Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        ysVector4 Direction = { 0.0f, 0.0f, -1.0f, 1.0f };
        float Attenuation0 = 0.0f;
        float Attenuation1 = 0.0f;
        int FalloffEnabled = 1; 
        int Active = 0;
    };

    struct LightingControls {
        static constexpr int MaxLights = 32;

        Light Lights[MaxLights];
        ysVector4 AmbientLighting;
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_SHADER_CONTROLS_H */
