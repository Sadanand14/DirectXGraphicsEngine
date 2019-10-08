SamplerState state  : register(s0);
Texture2D terrainTexture: register(t0);

struct VertexToPixel
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 sampleColor = terrainTexture.Sample(state,input.UV);
	sampleColor.w = 1.0f;
	return sampleColor;
}