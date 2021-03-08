#version 420

layout(location=0) in vec4 in_Position;
layout(location=1) in vec2 in_Tex;
layout(location=2) in vec4 in_Normal;

out vec4 ex_Pos;
out vec2 ex_Tex;

void main(void) {
	vec4 inputPos = vec4(in_Position.xyz, 1.0);

	ex_Pos = inputPos;
	ex_Tex = in_Tex;
	gl_Position = inputPos;
}
