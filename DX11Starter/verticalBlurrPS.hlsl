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
Texture2D blurrTex : register (t2);
SamplerState Sampler: register (s0);

float LinearEyeZ(float depth)
{
	float n = 0.1f;
	float f = 500.0f;
	float output = n / (f - depth * (f - n));
	return output;
}

float4 main(vertexToPixel  input) :SV_TARGET
{
	float4 color = float4(0.0f,0.0f,0.0f,1.0f);
	float4 color1 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 color2 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float pixelWidth = 1 / width;
	float pixelHeight = 1 / height;

	float sampleDepth = depthTex.Sample(Sampler, input.UV).x;
	float sampleSize = 40;
	float totalSamples = 0;

	for (unsigned int i = -sampleSize; i < sampleSize; i++)
	{
		int2 newUV = int2(input.position.x , input.position.y + i);
		color += blurrTex.Load(int3(newUV, 0));
		totalSamples++;
	}
	

	//for (unsigned int i = 0; i < sampleSize; i++)
	//{
	//	int2 newUV = int2(input.position.x, input.position.y + i);
	//	color2 += blurrTex.Load(int3(newUV, 0));
	//	totalSamples++;
	//}

	float4 blurrcolor =   color / totalSamples;

	//float temp = blurrcolor.x + blurrcolor.y + blurrcolor.z;
	float4 output = rawImage.Load(int3(input.position.x, input.position.y, 0));
	float depthSample = depthTex.Load(int3(input.position.x, input.position.y, 0)).x;
	depthSample = LinearEyeZ(depthSample);

	if ((depthSample > 0.21f) || ( depthSample < 0.095f))
	{
		output = blurrcolor;
	}

	return output;
	
}