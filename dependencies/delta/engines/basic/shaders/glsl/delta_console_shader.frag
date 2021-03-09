#version 420

layout(binding = 0) uniform sampler2D diffuseTex;

in vec4 ex_Pos;
in vec2 ex_Tex;
in vec4 ex_Col;

out vec4 out_Color;

layout (binding = 1) uniform ObjectVariables {
	vec4 MulCol;
	vec2 TexOffset;
	vec2 TexScale;
};

void main(void) {
	float a = texture(diffuseTex, ex_Tex).r;
	out_Color = vec4(1.0, 1.0, 1.0, a) * MulCol * ex_Col;
}
