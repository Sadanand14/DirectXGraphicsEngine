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
	float3 vsNormal		: NORMAL;
	float3 csPos		: CLIPPOS;
	matrix Proj			: PROJECTION;
};

VertexToPixel main(VertexShaderInput input)
{
	matrix WorldViewProj = mul(mul(world, view), projection);
	matrix WorldView = mul(world, view);

	VertexToPixel output;
	output.Position = mul(float4 (input.Position, 1.0f), WorldViewProj);
	output.vsNormal = mul(float4 (input.Normal, 1.0f), WorldView).xyz;
	//output.worldPos = mul(float4(input.Position, 1.0f), world).xyz;
	output.csPos = output.Position.xyz / output.Position.w;
	output.Proj = projection;
 	return output;
}
