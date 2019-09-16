cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 WaveParameters;//x = Amplitude ,y = omega, WaveLEngth
	float waterTime;
};

struct WaterVertex
{
	float3 Position	: POSITION;
	float3 Nomral	: NORMAL;
	float2 UV		: TEXCOORD;
	float3 Tangent	: TANGENT;
};

struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
	float Time  : TIME;
};

WaterVertexToPixel main(WaterVertex input)
{
	WaterVertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);

	// WAVE CALCULATIONS


	//float timestep = fmod(waterTime, 5.0f);
	//if (timestep > 3.0f)
	//{
	float temp = 3.14 * (waterTime - input.Position.x / 20);
	//float phaseStep = fmod(temp, 2 * 3.14);
	//if (temp > 2 * 3.14 && temp < 0) 
	//{
		input.Position.y = 4 * sin(temp);
	//}
	// moving water
		//input.Position.y = sin(3.14 * (waterTime *2 + input.Position.x / 20)) - sin(3.14* (waterTime*2 - ( input.Position.x)/20));// still water
	//}


	///////////////////

	output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	output.Time = waterTime;
	return output;
}

