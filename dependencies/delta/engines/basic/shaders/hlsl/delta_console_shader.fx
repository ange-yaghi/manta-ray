Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );

struct VS_OUTPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color: COLOR;
};

struct VS_INPUT_CONSOLE {
	float2 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color : COLOR;
};

cbuffer ScreenVariables : register(b0) {
	matrix CameraView;
	matrix Projection;
};

cbuffer ObjectVariables : register(b1) {
	float4 MulCol;
    float2 TexOffset;
	float2 TexScale;
};

VS_OUTPUT VS_CONSOLE(VS_INPUT_CONSOLE input) {
	float4 inputPos = float4(input.Pos, -1.0f, 1.0f);
	
	VS_OUTPUT output = (VS_OUTPUT) 0;
	inputPos = mul(inputPos, CameraView);
	inputPos = mul(inputPos, Projection);

	output.Pos = float4(inputPos.xyzw);

	input.TexCoord.y = input.TexCoord.y;
	output.TexCoord = ((input.TexCoord) * TexScale) + TexOffset;
	output.TexCoord = float2(output.TexCoord.x, output.TexCoord.y);

	output.Color = input.Color;

	return output;
}

float4 PS_CONSOLE(VS_OUTPUT input) : SV_Target {
	float a = txDiffuse.Sample(samLinear, input.TexCoord).r;
	return MulCol * float4(1.0, 1.0, 1.0, a) * input.Color;
}
