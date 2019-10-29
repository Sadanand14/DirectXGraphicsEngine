cbuffer externalData : register (b0) 
{
	float3 CameraPosition;
}

Texture2D SceneTex : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler : register (s0);

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	float4 worldPos		: POSITION;
	f
	matrix Proj			: PROJECTION;
	float2 UV			: TEXCOORD3;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	////ScreenSpace Reflection Stuff/////////////////////////////////////////////////////

	float4 ProjectionParams;
	ProjectionParams.x = 1 / input.Proj[0][0];
	ProjectionParams.y = 1 / input.Proj[1][1];
	ProjectionParams.z = input.Proj[3][2];
	ProjectionParams.w = input.Proj[2][2];
	
	float4 depthValue = depthTex.Sample(Sampler, input.UV);
	float linearDepth = ProjectionParams.z / (depthValue + ProjectionParams.w);

	float3 eyePosition = normalize(input.worldPos - CameraPosition);


	return depthValue;
}