cbuffer external : register(b0)
{
	matrix projection;
	matrix view;
}

struct vertexShaderInput 
{
	float3 position	: POSITION; 
	float4 Color	: COLOR;
	float2 UV		: TEXCOORD;
};


struct vertexToPixel 
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
	float4 Color	: COLOR;
};

vertexToPixel main(vertexShaderInput input)
{
	vertexToPixel output;

	matrix viewProj = mul(view, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	output.UV = input.UV;
	output.Color = input.Color;

	return output;
}