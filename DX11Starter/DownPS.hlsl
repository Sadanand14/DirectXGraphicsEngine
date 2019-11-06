cbuffer externalData : register (b0)
{
	float width;
	float height;
}
struct vertexToPixel 
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

Texture2D inputTex : register(t0);
Texture2D depthTex : register(t1);
SamplerState Sampler: register(s0);

float eyeZ(float depth) 
{
	float n = 0.1f;
	float f = 500.0f;
	float output = (n * f) / (f - depth * (f - n));
	return output;
}

float LinearZ(float depth) 
{
	float n = 0.1f;
	float f = 500.0f;
	float output = depth / (f - depth * (f - n));
	
	return output/10.0f;
}

float LinearEyeZ(float depth) 
{
	float n = 0.1f;
	float f = 500.0f;
	float output = n / (f - depth * (f - n));
	return output;
}

float4 main(vertexToPixel input) : SV_TARGET
{
	float4 color = float4(0.0f,0.0f,0.0f,0.0f);
	float sampleDepth = depthTex.Sample(Sampler, input.UV).x;

	float linearDepth = LinearEyeZ(sampleDepth);

	if (linearDepth > 0.2f || linearDepth <0.1)
	{
		color = inputTex.Sample(Sampler, input.UV);
	}
	return color;
}