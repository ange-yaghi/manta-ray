#version 420

layout(binding = 0) uniform sampler2D diffuseTex;

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;
in vec3 ex_Normal;

layout (binding = 0) uniform ScreenVariables {
	mat4 CameraView;
	mat4 Projection;
	vec4 CameraEye;
	
	vec4 FogColor;
	float FogNear;
	float FogFar;
};

layout (binding = 1) uniform ObjectVariables {
	mat4 Transform;
	vec2 TexOffset;
	vec2 TexScale;
	vec4 Scale;

	vec4 BaseColor;
	vec4 Emission;
	float SpecularMix;
	float DiffuseMix;
	float Metallic;
	float DiffuseRoughness;
	float SpecularPower;
	float IncidentSpecular;

	int ColorReplace;
	int Lit;
};

struct Light {
	vec4 Position;
	vec4 Color;
	vec4 Direction;
	float Attenuation0;
	float Attenuation1;
	int FalloffEnabled;
	int Active;
};

layout (binding = 3) uniform Lighting {
	Light Lights[32];
	vec4 AmbientLighting;
};

float pow5(float v) {
	return (v * v) * (v * v) * v;
}

float f_diffuse(vec3 i, vec3 o, vec3 h, vec3 normal, float power, float roughness) {
	float h_dot_i = dot(h, i);
	float h_dot_i_2 = h_dot_i * h_dot_i;
	float f_d90 = 0.5 + 2 * h_dot_i_2 * roughness;

	float cos_theta_i = dot(i, normal);
	float cos_theta_o = dot(o, normal);

	float f_d = (1 + (f_d90 - 1) * pow5(1 - cos_theta_i)) * (1 + (f_d90 - 1) * pow5(1 - cos_theta_o));
	return clamp(f_d * power * cos_theta_i, 0.0, 1.0);
}

float f_specular(vec3 i, vec3 o, vec3 h, vec3 normal, float F0, float power, float specularPower) {
	vec3 reflected = -reflect(i, normal);
	float intensity = dot(reflected, o);

	if (intensity < 0) return 0;

	// Fresnel approximation
	float F0_scaled = 0.08 * F0;
	float o_dot_h = dot(o, h);
	float s = pow5(1 - o_dot_h);
	float F = F0_scaled + s * (1 - F0_scaled);

	return clamp(pow(intensity, specularPower) * F * power, 0.0, 1.0);
}

float f_specular_ambient(vec3 o, vec3 normal, float F0, float power) {
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

vec3 linearToSrgb(vec3 v) {
	return vec3(
		linearToSrgb(v.r),
		linearToSrgb(v.g),
		linearToSrgb(v.b)
	);
}

vec3 srgbToLinear(vec3 v) {
	return vec3(
		srgbToLinear(v.r),
		srgbToLinear(v.g),
		srgbToLinear(v.b)
	);
}

void main(void) {
	const float FullSpecular = 1 / 0.08;

	vec3 totalLighting = vec3(1.0, 1.0, 1.0);
	vec3 normal = normalize(ex_Normal.xyz);

	vec4 baseColor;
	float roughness = 0.5;
	float power = 1.0;

	if (ColorReplace == 0) {
		vec4 diffuse = texture(diffuseTex, ex_Tex).rgba;
		baseColor = vec4(srgbToLinear(diffuse.rgb), diffuse.a) * BaseColor;
	}
	else {
		baseColor = BaseColor;
	}

	totalLighting = baseColor.rgb;

	if (Lit == 1) {
		vec3 o = normalize(CameraEye.xyz - ex_Pos.xyz);
		float cos_theta_o = dot(o, normal);

		vec3 ambientSpecular = f_specular_ambient(o, normal, IncidentSpecular, SpecularMix) * AmbientLighting.rgb;
		vec3 ambientDiffuse = f_diffuse(o, o, o, normal, DiffuseMix, DiffuseRoughness) * AmbientLighting.rgb * baseColor.rgb;
		vec3 ambientMetallic = f_specular_ambient(o, normal, FullSpecular, 1.0) * AmbientLighting.rgb * baseColor.rgb;

		totalLighting = mix(
			ambientSpecular + ambientDiffuse,
			ambientMetallic,
			Metallic);
		totalLighting += Emission.rgb;

		for (int li = 0; li < 32; ++li) {
			if (Lights[li].Active == 0) continue;

			vec3 i = Lights[li].Position.xyz - ex_Pos.xyz;
			float inv_dist = 1.0 / length(i);
			i *= inv_dist;

			float cos_theta_i = dot(i, normal);

			if (cos_theta_i < 0) continue;
			if (cos_theta_o < 0) continue;

			vec3 h = normalize(i + o);
			vec3 diffuse = f_diffuse(i, o, h, normal, DiffuseMix, DiffuseRoughness) * baseColor.rgb * Lights[li].Color.rgb;
			vec3 specular = f_specular(i, o, h, normal, IncidentSpecular, SpecularMix, SpecularPower) * Lights[li].Color.rgb;
			vec3 metallic = vec3(0.0, 0.0, 0.0);

			if (Metallic > 0) {
				metallic = f_specular(i, o, h, normal, FullSpecular, 1, SpecularPower) * Lights[li].Color.rgb * baseColor.rgb;
			}

			// Spotlight calculation
			float spotCoherence = -dot(i, Lights[li].Direction.xyz);
			float spotAttenuation = 1.0;
			if (spotCoherence > Lights[li].Attenuation0) spotAttenuation = 1.0;
			else if (spotCoherence < Lights[li].Attenuation1) spotAttenuation = 0.0;
			else {
				float t = Lights[li].Attenuation0 - Lights[li].Attenuation1;
				if (t == 0) spotAttenuation = 1.0;
				else spotAttenuation = (spotCoherence - Lights[li].Attenuation1) / t;
			}

			float falloff = 1.0;
			if (Lights[li].FalloffEnabled == 1) {
				falloff = (inv_dist * inv_dist);
			}

			vec3 bsdf = mix(
				diffuse + specular,
				metallic,
				Metallic);

			totalLighting += falloff * bsdf * spotAttenuation * spotAttenuation * spotAttenuation;
		}
	}

	const float distanceToCamera = length(CameraEye.xyz - ex_Pos.xyz);
	const float fogAttenuation = (clamp(distanceToCamera, FogNear, FogFar) - FogNear) / (FogFar - FogNear);

	out_Color = vec4(
		linearToSrgb(
			mix(
				totalLighting.rgb,
				FogColor.rgb,
				fogAttenuation)), baseColor.a);
}
