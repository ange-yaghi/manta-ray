#version 420

layout(binding = 0) uniform sampler2D inputBuffer;

out vec4 out_Color;

in vec4 ex_Pos;
in vec2 ex_Tex;
in vec3 ex_Normal;

void main(void) {
	out_Color = texture(inputBuffer, ex_Tex).rgba;
}
