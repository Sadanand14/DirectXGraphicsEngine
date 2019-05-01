
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{ 
	float3 position		: POSITION;     
	float3 Normal       : NORMAL;
	float3 Tangent		: TANGENT;
	float2 UV           : TEXCOORD;
};

struct VertexToPixel
{
	float4 screenPos	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 UV			: TEXCOORD;
};

VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);
	output.screenPos = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.position, 1.0f), world);
	output.Normal = mul(input.Normal,(float3x3)world);
	output.Tangent = normalize(mul(input.Tangent, (float3x3)world));

	output.UV = input.UV;
	
	return output;
}