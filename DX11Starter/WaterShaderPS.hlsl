struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

Texture2D waterTexture : register (t0);
SamplerState Sampler   : register (s0);

float4 main(WaterVertexToPixel input) : SV_TARGET
{
	float4 finalColor = waterTexture.Sample(Sampler ,input.UV);
	return finalColor;
}