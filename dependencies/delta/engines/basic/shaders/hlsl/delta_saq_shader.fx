Texture2D layer0 : register( t0 );
Texture2D layer1 : register( t1 );
SamplerState samLinear : register( s0 );

struct VS_INPUT_STANDARD {
	float4 Pos : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Normal : NORMAL;
};

struct VS_OUTPUT {
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

VS_OUTPUT VS_SAQ(VS_INPUT_STANDARD input) {
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = float4(input.Pos.xyz, 1.0f);
	output.TexCoord = float2(input.TexCoord.x, 1.0f - input.TexCoord.y);

	return output;
}

float4 PS_SAQ(VS_OUTPUT input) : SV_Target {
	float4 l0 = layer0.Sample(samLinear, input.TexCoord);
	float4 l1 = layer1.Sample(samLinear, input.TexCoord);
	float alpha = l1.a;

	return (alpha * l1) + (1 - alpha) * l0;
}
