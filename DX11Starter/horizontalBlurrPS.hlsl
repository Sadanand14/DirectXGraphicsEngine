cbuffer external : register(b0) 
{
	float width;
	float height;
}

struct vertexToPixel
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

Texture2D rawImage : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler: register (s0);

float4 main(vertexToPixel  input) :SV_TARGET
{
	float4 color = float4(0.0f,0.0f,0.0f,0.0f);
	float pixelWidth  = 1 / width;
	float pixelHeight = 1 / height;

	//float sampleDepth = depthTex.Sample(Sampler, input.UV).x;
	float sampleSize = 4;
	float totalSamples = 0;
	for (unsigned int i = -sampleSize; i <= sampleSize; i++)
	{
		int2 newUV = int2(input.position.x + i, input.position.y);
		color += rawImage.Load(int3(newUV,0));
		totalSamples++;
	}
	return color/totalSamples;
}