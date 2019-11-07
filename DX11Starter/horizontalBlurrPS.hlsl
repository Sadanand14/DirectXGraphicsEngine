cbuffer external : register(b0) 
{
	float depths[8];
	float width;
	float height;
	float lowerFocusDepth;
	float upperFocusDepth;
	int StepSize;
}

struct vertexToPixel
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

Texture2D rawImage : register (t0);
Texture2D depthTex : register (t1);
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
	float4 color = float4(0.0f,0.0f,0.0f,0.0f);

	// set depthStep
	int depthSize = 2;
	//Sample current Depth
	float sampleDepth = depthTex.Sample(Sampler, input.UV).x;
	sampleDepth = LinearEyeZ(sampleDepth);

	//calculate the minimum depth difference between the focus area and the current pixel.
	float depthRange = min(abs(lowerFocusDepth - sampleDepth), abs(upperFocusDepth - sampleDepth));

	//iterate through the depths in the array to find a matching range.
	unsigned int k;
	for (k = 0; k <8; k++)
	{
		if ((depths[k] - depthRange) > 0.0f)
		{
			break;
		}
	}

	// apply sample Size based on the depth
	float sampleSize = depthSize *(++k);
	//float sampleSize = 10;
	float totalSamples = 0;
	float currDepth;
	for (unsigned int i = -sampleSize; i <= sampleSize; i++)
	{
		int2 newUV = int2(input.position.x, input.position.y + i);
		currDepth = depthTex.Load(int3(newUV, 0)).x;
		currDepth = LinearEyeZ(currDepth);
		float4 rawColor = rawImage.Load(int3(newUV, 0));
		//float temp = rawColor.x + rawColor.y + rawColor.z;*/
		if ((abs(sampleDepth-currDepth)<0.00005f))
		{
			color += rawColor;
			totalSamples++;
		}

	}
	
	return color/totalSamples;
}