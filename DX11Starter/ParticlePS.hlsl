cbuffer external : register (b0)
{
	int debugWireFrame;
}

struct vertexToPixel
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
	float4 Color	: COLOR;
};

Texture2D particleTex : register (t0);
SamplerState Sampler : register (s0);

float4 main(vertexToPixel input) : SV_TARGET
{
	float4 Color = particleTex.Sample(Sampler, input.UV) * input.Color;

	return lerp(Color, float4(1.0f,1.0f,1.0f,0.25f), debugWireFrame);
}