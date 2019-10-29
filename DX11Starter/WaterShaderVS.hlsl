cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	float waterTime;
};



struct Wave 
{
	float2 Direction;
	float Amplitude;
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
	float4 Position			: SV_POSITION;
	float3 Normal			: NORMAL;
	float2 UV				: TEXCOORD;
	float3 Tangent			: TANGENT;
	float3 worldPos			: POSITION;
	matrix View				: VIEW;
	noperspective float2 ScreenUV : TEXCOORD1;
};

//Calculates Position using Gerstner Equation
float3 CalculateWavePosition(float3 inputPosition, int length)
{
	float3 finalPosition = inputPosition;
	float3 Speed = 5.0f;
	float steepness = 1.2;
	float depth = 0.5;
	float WVT = 1;
	float SVT = 0.5;
	float pi = 3.14;

	for (unsigned int i = 0; i < length; i++) 
	{
		float2 Di = waves[i].Direction;
		float Li = waves[i].Wavelength;
		float Ai = waves[i].Amplitude;
		float Si = Speed;
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

		float sineValue = sin(Wi * dot(inputPosition.xz, normalize(Di)) + phase * waterTime);
		float finalValue = pow((sineValue + 1) / 2, steepness);

		finalPosition.y += Ai * finalValue;

	}

	return finalPosition;
}

//Calculates Gerstner Normals
float3 UpdateNormals(float3 inputPosition,int length)
{
	float steepness =  0.9f;
	float3 baseNormal = float3(0, 1, 0);
	float3 Speed = 5.0f;
	for (int i = 0; i < length; i++) 
	{
		float2 Di = waves[i].Direction;
		float Ai = waves[i].Amplitude;
		float Li = waves[i].Wavelength;
		float Si = Speed;
		float Wi = 2 / Li;
		float phase = Wi * Si;

		baseNormal.x -= steepness* Ai * Ai * cos(dot(inputPosition.xz, normalize(Di)) * Wi + phase * waterTime);
		baseNormal.z -= steepness* Ai * Ai * cos(dot(inputPosition.xz, normalize(Di)) * Wi + phase * waterTime);
	}

	return normalize(baseNormal);
}

//Calculates Gerstner Tangent
float3 UpdateTangents(float3 inputPosition, int length) 
{
	float3 Speed = 5.0f;      
	float3 baseTangent = float3(0, 0, 1);
	for (int i = 0; i < length; i++) 
	{
		float2 Di = waves[i].Direction;
		float Ai = waves[i].Amplitude;
		float Li = waves[i].Wavelength;
		float Si = Speed;
		float Wi = 2 / Li;
		float phase = Wi * Si;

		baseTangent.z -= Ai * Ai * cos(dot(inputPosition.xz, normalize(Di)) * Wi + phase * waterTime);
	}

	return normalize(baseTangent);
}

WaterVertexToPixel main(WaterVertex input)
{
	WaterVertexToPixel output;
	matrix worldView = mul(world, view);
	matrix worldViewProj = mul(worldView, projection);

	//input.Position.y -= 10.0f;
	// WAVE CALCULATIONS///////////////////////////
	
	//input.Position = CalculateWavePosition(input.Position, 8);
	//input.Normal = UpdateNormals(input.Position, 8); 
	//input.Tangent = UpdateTangents(input.Position, 8);

	///////////////////////////////////////////////
	output.View = view;
	output.Position = mul(float4(input.Position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.Position, 1.0f), world).xyz;
	output.UV = input.UV;

	output.Normal = mul(input.Normal, (float3x3)world);
	output.Normal = normalize(input.Normal);

	output.Tangent = normalize(mul(input.Tangent, (float3x3)world));

	output.ScreenUV = (output.Position.xy / output.Position.w);
	output.ScreenUV.x = output.ScreenUV.x * 0.5f + 0.5f;
	output.ScreenUV.y = -output.ScreenUV.y * 0.5f + 0.5f;

	return output;
}

