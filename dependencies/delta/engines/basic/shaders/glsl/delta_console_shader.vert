#version 420

layout(location=0) in vec2 in_Position;
layout(location=1) in vec2 in_Tex;
layout(location=2) in vec4 in_Color;

out vec4 ex_Pos;
out vec2 ex_Tex;
out vec4 ex_Col;

layout (binding = 0) uniform ScreenVariables {
	mat4 CameraView;
	mat4 Projection;
};

layout (binding = 1) uniform ObjectVariables {
	vec4 MulCol;
	vec2 TexOffset;
	vec2 TexScale;
};

void main(void) {
	vec4 inputPos = vec4(in_Position, -1.0, 1.0);

	inputPos = inputPos * CameraView;
    inputPos = inputPos * Projection;

	ex_Pos = inputPos;
	ex_Tex = in_Tex * TexScale + TexOffset;
	ex_Col = in_Color;

	gl_Position = inputPos;
}
