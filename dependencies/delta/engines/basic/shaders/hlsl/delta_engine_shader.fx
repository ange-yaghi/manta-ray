Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

struct VS_OUTPUT {
	float4 Pos : SV_POSITION;
	float4 VertexPosition : VERTEX_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL;

	float BoneWeight : BONE_WEIGHT;
};

struct VS_INPUT_SKINNED {
	float4 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Normal : NORMAL;

	int4 BoneIndices : BONE_INDICES;
	float4 BoneWeights : BONE_WEIGHTS;
};

struct VS_INPUT_STANDARD {
	float4 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Normal : NORMAL;
};

cbuffer ScreenVariables : register(b0) {
	matrix CameraView;
	matrix Projection;
	float4 CameraEye;

	float4 FogColor;
	float FogNear;
	float FogFar;
};

cbuffer ObjectVariables : register(b1) {
	matrix Transform;
	float2 TexOffset;
	float2 TexScale;
	float4 Scale;

	float4 BaseColor;
	float4 Emission;
	float SpecularMix;
	float DiffuseMix;
	float Metallic;
	float DiffuseRoughness;
	float SpecularPower;
	float IncidentSpecular;

	int ColorReplace;
	int Lit;
};

cbuffer SkinningVariables : register(b2) {
	matrix BoneTransform[256];
};

struct Light {
	float4 Position;
	float4 Color;
	float4 Direction;
	float Attenuation0;
	float Attenuation1;
	int FalloffEnabled;
	int Active;
};

cbuffer Lighting : register(b3) {
	Light Lights[32];
	float4 AmbientLighting;
};

VS_OUTPUT VS_SKINNED(VS_INPUT_SKINNED input) {
	VS_OUTPUT output = (VS_OUTPUT) 0;

	float4 inputPos = float4(input.Pos.xyz, 1.0);

	float4 final = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 finalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);

	output.Normal = mul(input.Normal, Transform).xyz;
	inputPos = mul(inputPos, Transform);

	if (input.BoneIndices.x >= 0) {
		final += mul(BoneTransform[input.BoneIndices.x], inputPos) * input.BoneWeights.x;
		finalNormal += mul(BoneTransform[input.BoneIndices.x], output.Normal) * input.BoneWeights.x;

		if (input.BoneIndices.y >= 0) {
			final += mul(BoneTransform[input.BoneIndices.y], inputPos) * input.BoneWeights.y;
			finalNormal += mul(BoneTransform[input.BoneIndices.y], output.Normal) * input.BoneWeights.y;

			if (input.BoneIndices.z >= 0) {
				final += mul(BoneTransform[input.BoneIndices.z], inputPos) * input.BoneWeights.z;
				finalNormal += mul(BoneTransform[input.BoneIndices.z], output.Normal) * input.BoneWeights.z;
			}
		}
	}
	else {
		final = inputPos;
		finalNormal = float4(output.Normal, 1.0);
	}

	output.BoneWeight = 0.0;

	inputPos = float4(final.xyz, 1.0);
	
	inputPos = mul(inputPos, Transform);
	inputPos.xyz *= Scale.xyz;
	inputPos = mul(inputPos, CameraView);
	inputPos = mul(inputPos, Projection);

	output.Pos = float4(inputPos.xyzw);

	input.TexCoord.y = 1 - input.TexCoord.y;
	output.TexCoord = ( (input.TexCoord) * TexScale) + TexOffset;
	output.TexCoord = float2(output.TexCoord.x, output.TexCoord.y);

	output.Normal = finalNormal.xyz;

	return output;
}

VS_OUTPUT VS_STANDARD(VS_INPUT_STANDARD input) {
	float4 inputPos = float4(input.Pos);
	
	VS_OUTPUT output = (VS_OUTPUT) 0;

	float4 final = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 finalNormal = float4(0.0f, 0.0f, 0.0f, 0.0f);

	output.Normal = mul(input.Normal, Transform).xyz;

	final = inputPos;
	finalNormal = float4(output.Normal, 1.0);

	inputPos = float4(final.xyz, 1.0);
	
	inputPos.xyz *= Scale.xyz;
	inputPos = mul(inputPos, Transform);
	output.VertexPosition = inputPos;

	inputPos = mul(inputPos, CameraView);
	inputPos = mul(inputPos, Projection);

	output.Pos = float4(inputPos.xyzw);

	input.TexCoord.y = 1 - input.TexCoord.y;
	output.TexCoord = ( (input.TexCoord) * TexScale) + TexOffset;
	output.TexCoord = float2(output.TexCoord.x, output.TexCoord.y);

	output.Normal = finalNormal;
	output.BoneWeight = 0.0;

	return output;
}

float pow5(float v) {
	return (v * v) * (v * v) * v;
}

float f_diffuse(float3 i, float3 o, float3 h, float3 normal, float power, float roughness) {
	float h_dot_i = dot(h, i);
	float h_dot_i_2 = h_dot_i * h_dot_i;
	float f_d90 = 0.5 + 2 * h_dot_i_2 * roughness;

	float cos_theta_i = dot(i, normal);
	float cos_theta_o = dot(o, normal);

	float f_d = (1 + (f_d90 - 1) * pow5(1 - cos_theta_i)) * (1 + (f_d90 - 1) * pow5(1 - cos_theta_o));
	return clamp(f_d * power * cos_theta_i, 0.0, 1.0);
}

float f_specular(float3 i, float3 o, float3 h, float3 normal, float F0, float power, float specularPower) {
	float3 reflected = -reflect(i, normal);
	float intensity = dot(reflected, o);

	if (intensity < 0) return 0;

	// Fresnel approximation
	float F0_scaled = 0.08 * F0;
	float o_dot_h = dot(o, h);
	float s = pow5(1 - o_dot_h);
	float F = F0_scaled + s * (1 - F0_scaled);

	return clamp(pow(intensity, specularPower) * F * power, 0.0, 1.0);
}

float f_specular_ambient(float3 o, float3 normal, float F0, float power) {
	// Fresnel approximation
	float F0_scaled = 0.08 * F0;
	float o_dot_n = dot(o, normal);
	float s = pow5(1 - o_dot_n);
	float F = F0_scaled + s * (1 - F0_scaled);

	return clamp(F * power, 0.0, 1.0);
}

float linearToSrgb(float u) {
	const float MinSrgbPower = 0.0031308;

	if (u < MinSrgbPower) {
		return 12.92 * u;
	}
	else {
		return 1.055 * pow(u, 1 / 2.4) - 0.055;
	}
}

float srgbToLinear(float u) {
	const float MinSrgbPower = 0.04045;

    if (u < MinSrgbPower) {
        return u / 12.92;
    }
    else {
        return pow((u + 0.055) / 1.055, 2.4);
    }
}

float3 linearToSrgb(float3 v) {
	return float3(
		linearToSrgb(v.r),
		linearToSrgb(v.g),
		linearToSrgb(v.b)
	);
}

float3 srgbToLinear(float3 v) {
	return float3(
		srgbToLinear(v.r),
		srgbToLinear(v.g),
		srgbToLinear(v.b)
	);
}

float4 PS(VS_OUTPUT input) : SV_Target {
	const float FullSpecular = 1 / 0.08;

	float3 totalLighting = float3(1.0, 1.0, 1.0);
	float3 normal = normalize(input.Normal);

	float4 baseColor;
	float roughness = 0.5;
	float power = 1.0;

	if (ColorReplace == 0) {
		float4 diffuse = txDiffuse.Sample(samLinear, input.TexCoord).rgba;
		baseColor = float4(srgbToLinear(diffuse.rgb), diffuse.a) * BaseColor;
	}
	else {
		baseColor = BaseColor;
	}

	totalLighting = baseColor.rgb;

	if (Lit == 1) {
		float3 o = normalize(CameraEye.xyz - input.VertexPosition.xyz);
		float cos_theta_o = dot(o, normal);

		float3 ambientSpecular = f_specular_ambient(o, normal, IncidentSpecular, SpecularMix) * AmbientLighting;
		float3 ambientDiffuse = f_diffuse(o, o, o, normal, DiffuseMix, DiffuseRoughness) * AmbientLighting * baseColor;
		float3 ambientMetallic = f_specular_ambient(o, normal, FullSpecular, 1.0) * AmbientLighting.rgb * baseColor.rgb;

		totalLighting = lerp(
			ambientSpecular + ambientDiffuse,
			ambientMetallic,
			Metallic);
		totalLighting += Emission.rgb;

		for (int li = 0; li < 32; ++li) {
			const Light light = Lights[li];
			if (light.Active == 0) continue;

			float3 i = light.Position.xyz - input.VertexPosition.xyz;
			float inv_mag = 1.0 / length(i);
			i *= inv_mag;

			float cos_theta_i = dot(i, normal);

			if (cos_theta_i < 0) continue;

			float3 h = normalize(i + o);
			float3 diffuse = f_diffuse(i, o, h, normal, DiffuseMix, DiffuseRoughness) * baseColor.rgb * light.Color;
			float3 specular = f_specular(i, o, h, normal, IncidentSpecular, SpecularMix, SpecularPower) * light.Color;
			float3 metallic = 0;

			if (Metallic > 0) {
				metallic = f_specular(i, o, h, normal, FullSpecular, 1, SpecularPower) * light.Color * baseColor.rgb;
			}

			// Spotlight calculation
			float spotCoherence = -dot(i, light.Direction.xyz);
			float spotAttenuation = 1.0f;
			if (spotCoherence >= light.Attenuation0) spotAttenuation = 1.0f;
			else if (spotCoherence < light.Attenuation1) spotAttenuation = 0.0f;
			else {
				float t = light.Attenuation0 - light.Attenuation1;
				if (t == 0) spotAttenuation = 1.0f;
				else spotAttenuation = (spotCoherence - light.Attenuation1) / t;
			}

			float falloff = 1.0;
			if (light.FalloffEnabled == 1) {
				falloff = (inv_mag * inv_mag);
			}

			float3 bsdf = lerp(
				diffuse + specular,
				metallic, 
				Metallic);

			totalLighting += falloff * bsdf * spotAttenuation * spotAttenuation * spotAttenuation;
		}
	}

	const float distanceToCamera = length(CameraEye.xyz - input.VertexPosition.xyz);
	const float fogAttenuation = (clamp(distanceToCamera, FogNear, FogFar) - FogNear) / (FogFar - FogNear);

	return float4(
		linearToSrgb(
			lerp(
				totalLighting.rgb,
				FogColor.rgb,
				fogAttenuation)), baseColor.a);
}
