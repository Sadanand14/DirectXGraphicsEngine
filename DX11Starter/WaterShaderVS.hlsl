cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 WaveParameters;//x = Amplitude ,y = omega, WaveLEngth
	float waterTime;
};


struct Wave 
{
	float Amplitude;
	float Frequency;
	float Speed;
	float Wavelength;
};

cbuffer WaveData : register(b1)
{
	Wave waves[8];
}

struct WaterVertex
{
	float3 Position	: POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
	float3 Tangent	: TANGENT;
};

struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

float3 CalculateWavePosition(float3 inputPosition, int length)
{
	float3 finalPosition = inputPosition;
	for (unsigned int i = 0; i < length; i++) 
	{
		finalPosition.x += waves[i].Amplitude * sin((waves[i].Frequency * finalPosition.x) + (waterTime * waves[i].Speed / (2 * waves[i].Wavelength)));
		finalPosition.z += waves[i].Amplitude * sin((waves[i].Frequency * finalPosition.z) + (waterTime * waves[i].Speed / (2 * waves[i].Wavelength)));
		//finalPosition.y += waves[i].Amplitude * sin(waves[i].Frequency * finalPosition.y + waterTime * waves[i].Speed / (2 * waves[i].Wavelength));
		finalPosition.y += waves[i].Amplitude * sin(2 * 3.14 * finalPosition.y * waves[i].Frequency * waterTime);
	}
	return finalPosition;
}

WaterVertexToPixel main(WaterVertex input)
{
	WaterVertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);

	// WAVE CALCULATIONS
	
	
	//input.Position = CalculateWavePosition(input.Position, 8);
	

	float Amplitude = 2.0f;
	float frequency = 0.2f;
	float Speed = 5.0f;
	float Wavelength = 2.0f;

	float Amplitude1 = 1.0f;
	float frequency1 = 0.4f;
	float Speed1 = 1.0f;
	float Wavelength1 = 1.0f;

    input.Position.y = input.Position.y + Amplitude * sin(2 * 3.14 * input.Position.y* frequency*waterTime) + Amplitude1 * sin(2 * 3.14 * frequency1 * waterTime);
	input.Position.x = input.Position.x + Amplitude * sin(frequency* input.Position.x + waterTime * Speed / (4.0f)) + Amplitude1 * sin(frequency1* input.Position.x + waterTime * Speed1 / (2.0f));
	input.Position.z = input.Position.z + Amplitude * sin(frequency* input.Position.z + waterTime * Speed / (4.0f)) + Amplitude1 * sin(frequency1* input.Position.z+ waterTime * Speed1 / (2.0f));
	///////////////////
	
	output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	output.UV = input.UV;
	return output;
}

