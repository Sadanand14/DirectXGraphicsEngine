cbuffer external : register(b0)
{
	float depths[4];
	float width;
	float height;
	float upperFocusDepth;
	float lowerFocusDepth;
	int StepSize;
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
	int depthSize = 1;
	float sampleDepth = depthTex.Load(int3(input.position.xyz)).x;
	sampleDepth = LinearEyeZ(sampleDepth);
	float depthRange = min(abs(lowerFocusDepth - sampleDepth), abs(upperFocusDepth - sampleDepth));

	unsigned int k;
	for (k = 0; k < 4; k++)
	{
		if ((depths[k] - depthRange ) > 0.0f)
		{
			break;
		}
	}
	float sampleSize = depthSize * (++k);
	//float sampleSize = 1;
	float totalSamples = 0;
	float currDepth;
	for (unsigned int i = -sampleSize; i < sampleSize; i++)
	{
		int2 newUV = int2(input.position.x , input.position.y + i);
		//currDepth = depthTex.Load(int3(newUV, 0)).x;
		//currDepth = LinearEyeZ(currDepth);
		float4 rawColor;
		rawColor = blurrTex.Load(int3(newUV, 0));
		float temp = rawColor.x + rawColor.y + rawColor.z;
		if (temp > 0) 
		{
			color += rawColor;
			totalSamples++;
		}
	}

	float4 blurrcolor =   color / totalSamples;

	//float temp = blurrcolor.x + blurrcolor.y + blurrcolor.z;
	float4 output = rawImage.Load(int3(input.position.x, input.position.y, 0));
	//sampleDepth = LinearEyeZ(sampleDepth);

	if ((sampleDepth > 0.21f) || (sampleDepth < 0.095f))
	{
		output = blurrcolor;
	}
	//return float4(((float)StepSize) / 10, ((float)StepSize) / 10, ((float)StepSize) / 10, 1.0f);
	//return float4(((float)StepSize) / 10, ((float)StepSize) / 10, ((float)StepSize) / 10, 1.0f);
	return output;
	
}