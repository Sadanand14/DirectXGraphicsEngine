cbuffer externalData : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VertexShaderInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 UV		: TEXCOORD;
	float3 Tangent	: TANGENT;
};

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	float3 worldPos		: POSITION;
	float3 Normal
	matrix Proj			: PROJECTION;
	float2 UV			: TEXCOORD3;
};

VertexToPixel main(VertexShaderInput input)
{
	matrix WorldViewProj = mul(mul(world, view), projection);
	matrix WorldView = mul(world, view);

	VertexToPixel output;
	output.Position = mul(float4 (input.Position, 1.0f), WorldViewProj);
	output.worldPos = mul(float4(input.Position, 1.0f), world).xyz;
	output.Proj = projection;
 	return output;
}
