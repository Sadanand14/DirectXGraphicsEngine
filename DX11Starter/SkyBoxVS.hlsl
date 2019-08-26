cbuffer externalData: register(b0)
{
	matrix view;
	matrix projection;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 direction	: NORMAL;
};

struct Input
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float2 UV		: TEXCOORD;
};

VertexToPixel main(Input input)
{
	VertexToPixel output;

	matrix viewNotrans = view;
	viewNotrans._41 = 0;
	viewNotrans._42 = 0;
	viewNotrans._43 = 0;

	matrix vp = mul(viewNotrans, projection);

	output.position = mul(float4(input.Position, 1.0f), vp);

	output.position.z = output.position.w;

	output.direction = input.Position;

	return output;
}