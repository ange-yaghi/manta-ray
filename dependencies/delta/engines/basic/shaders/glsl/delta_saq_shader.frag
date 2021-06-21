#version 420

layout(binding = 0) uniform sampler2D layer0;
layout(binding = 1) uniform sampler2D layer1;

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;
in vec3 ex_Normal;

void main(void) {
	vec4 l0_color = texture(layer0, ex_Tex).rgba;
	vec4 l1_color = texture(layer1, ex_Tex).rgba;
	float alpha = l1_color.a;

	out_Color = alpha * l1_color + (1 - alpha) * l0_color;
}
