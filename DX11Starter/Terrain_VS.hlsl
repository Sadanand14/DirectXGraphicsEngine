cbuffer externalData: register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VertexShaderInput
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

struct VertexToPixel
{
	float4 Position : SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel Output;
	matrix worldViewProj = mul(mul(world, view), projection);

	Output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	Output.Normal = mul(input.Normal, (float3x3)world);
	Output.UV = input.UV;
	return Output;
}