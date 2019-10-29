cbuffer externalData : register (b0)
{
	matrix world;
	matrix view;
	matrix projection;
	//matrix invView;
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
	float4 vsPos		: TEXCOORD0;
	float3 csPos		: TEXCOORD1;
	float3 vsNormal		: TEXCOORD2;
	//float3 vsUp			: TEXCOORD3;
	matrix Proj			: PROJECTION;
};

VertexToPixel main(VertexShaderInput input)
{
	matrix WorldViewProj = mul(mul(world, view), projection);
	matrix WorldView = mul(world, view);

	VertexToPixel output;
	output.Position = mul(float4 (input.Position, 1.0f), WorldViewProj);
	output.vsPos = mul(float4 (input.Position, 1.0f), WorldView);
	output.vsNormal = mul(input.Normal, (float3x3)WorldView);
	output.csPos = output.Position.xyz / output.Position.w;
	output.Proj = projection;
	//output.vsUp = mul(float3(0.0f,1.0f,0.0f), (float3x3) view);
 	return output;
}
