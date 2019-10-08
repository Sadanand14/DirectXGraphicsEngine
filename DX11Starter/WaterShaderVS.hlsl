cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float4 WaveParameters;//x = Amplitude ,y = omega, WaveLEngth
	float waterTime;
};

float pi = 3.14;


struct Wave 
{
	float Amplitude;
	float Wavelength;
	float Speed;
	float garbage;
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
	float3 Normal	: Normal;
	float2 UV		: TEXCOORD;
};

//Calculates Position using Gerstner Equation
float3 CalculateWavePosition(float3 inputPosition, int length)
{
	float3 finalPosition = inputPosition;
	float steepness = 1.2;
	float depth = 5;
	float WVT = 1;
	float SVT = 0.5;
<<<<<<< HEAD

	for (unsigned int i = 0; i < length/2; i++) 
=======
	float pi = 3.14;

	for (unsigned int i = 0; i < length/2; i++) 
	{
		float Li = waves[i].Wavelength;
		float Ai = waves[i].Amplitude;
		float Si = waves[i].Speed;
		float Wi = 2 /Li ;


		float phase = Si * Wi;

		//adjustWavelength
		Li = Li - WVT + (2 * depth * WVT) / Li;
		Wi = 2 / Li;

		//AdjustSteepness
		steepness = steepness - SVT + (2 * depth * SVT) / steepness;

		finalPosition.x += Ai * sin(Wi * inputPosition.x + phase * waterTime);
		finalPosition.z += Ai * sin(Wi * inputPosition.z + phase * waterTime);

		//float xDirection = pow(-1, i);

		float sineValue = sin(Wi * dot(inputPosition.xz, normalize(float2(1, -1))) + phase * waterTime);
		float finalValue = pow((sineValue + 1) / 2, steepness);

		finalPosition.y += Ai * finalValue;

	}

	for (unsigned int i = length/2; i < length; i++)
>>>>>>> 607ec4e818de825e88055f1e040a2a9f65c26a1c
	{
		float Li = waves[i].Wavelength;
		float Ai = waves[i].Amplitude;
		float Si = waves[i].Speed;
<<<<<<< HEAD
		float Wi = 2 /Li ;


		float phase = Si * Wi;

		//adjustWavelength
		Li = Li - WVT + (2 * depth * WVT) / Li;
		Wi = 2 / Li;

		//AdjustSteepness
		steepness = steepness - SVT + (2 * depth * SVT) / steepness;

		finalPosition.x += Ai * sin(Wi * inputPosition.x + phase * waterTime);
		finalPosition.z += Ai * sin(Wi * inputPosition.z + phase * waterTime);

		//float xDirection = pow(-1, i);

		float sineValue = sin(Wi * dot(inputPosition.xz, normalize(float2(1, -1))) + phase * waterTime);
		float finalValue = pow((sineValue + 1) / 2, steepness);

		finalPosition.y += Ai * finalValue;

	}

	for (unsigned int i = length/2; i < length; i++)
	{
		float Li = waves[i].Wavelength;
		float Ai = waves[i].Amplitude;
		float Si = waves[i].Speed;
=======
>>>>>>> 607ec4e818de825e88055f1e040a2a9f65c26a1c
		float Wi = 2 / Li;

		float phase = Si * Wi;

		//adjustWavelength
		Li = Li - WVT + (2 * depth * WVT)/Li;
		Wi = 2 / Li;
		
		//AdjustSteepness
		steepness = steepness - SVT + (2 * depth * SVT) / steepness;

		finalPosition.x += Ai * sin(Wi * inputPosition.x + phase * waterTime);
		finalPosition.z += Ai * sin(Wi * inputPosition.z + phase * waterTime);

		//float xDirection = pow(-1, i);

		float sineValue = sin(Wi * dot(inputPosition.xz, normalize(float2(-1, -1))) + phase * waterTime);
		float finalValue = pow((sineValue + 1) / 2, steepness);

		finalPosition.y += Ai * finalValue;

	}

	return finalPosition;
}

//Calculates Gerstner Normals
<<<<<<< HEAD
float3 UpdateNormals(float3 inputPosition,int length)
{
	float3 baseNormal = float3(0, 1, 0);

	for (int i = 0; i < length; i++) 
=======
float3 UpdateNormals(float3 inputPosition, WaterVertexToPixel output,int length)
{
	float pi = 3.14;
	float3 baseNormal = float3(0, 1, 0);

	for (int i = 0; i < length/2; i++) 
>>>>>>> 607ec4e818de825e88055f1e040a2a9f65c26a1c
	{
		float Ai = waves[i].Amplitude;
		float Li = waves[i].Wavelength;
		float Si = waves[i].Speed;
		float Wi = 2 / Li;
		float phase = Wi * Si;
		float2 direction = float2(1, -1);

<<<<<<< HEAD
		baseNormal.x -= Ai * cos(dot(inputPosition.xz, normalize(direction)) * Wi + phase * waterTime);
		baseNormal.z -= Ai * cos(dot(inputPosition.xz, normalize(direction)) * Wi + phase * waterTime);
	}

	return normalize(baseNormal);
}

//Calculates Gerstner Tangent
float3 UpdateTangents(float3 inputPosition, int length) 
{
	float3 baseTangent = float3(0, 0, 1);
	for (int i = 0; i < length; i++) 
	{
		float Ai = waves[i].Amplitude;
		float Li = waves[i].Wavelength;
		float Si = waves[i].Speed;
		float Wi = 2 / Li;
		float phase = Wi * Si;
		float2 direction = float2(1, -1);

		baseTangent.y += Ai * cos(dot(inputPosition.xz, normalize(direction)) * Wi + phase * waterTime);
	}

	return normalize(baseTangent);
=======
		baseNormal.x -= Ai * cos(dot(inputPosition.xz, normalize(float2(1,-1))) * Wi + phase * waterTime);
		baseNormal.z -= Ai * cos(dot(inputPosition.xz, normalize(direction)) * Wi * ( -1) + phase * waterTime);

		baseNormal.y += Ai * sin(dot(inputPosition.xz, normalize(direction)) * Wi + phase * waterTime);
	}

>>>>>>> 607ec4e818de825e88055f1e040a2a9f65c26a1c
}

WaterVertexToPixel main(WaterVertex input)
{
	WaterVertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);


<<<<<<< HEAD
	// WAVE CALCULATIONS///////////////////////////
	
	input.Position = CalculateWavePosition(input.Position, 8);
	input.Normal = UpdateNormals(input.Position, 8);
	input.Tangent = UpdateTangents(input.Position, 8);

	///////////////////////////////////////////////
=======
	// WAVE CALCULATIONS
	
	
	input.Position = CalculateWavePosition(input.Position, 8);
	
	///////////////
>>>>>>> 607ec4e818de825e88055f1e040a2a9f65c26a1c
	
	output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	output.UV = input.UV;
	return output;
}

