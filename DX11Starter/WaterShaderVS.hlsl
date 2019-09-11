cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct WaterVertex
{
	float3 Position	: POSITION;
	float3 Nomral	: NORMAL;
	float2 UV		: TEXCOORD;
	float3 Tangent	: TANGENT;
};

struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
};

WaterVertexToPixel main(WaterVertex input) 
{
	WaterVertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);
	output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	return output;
}

