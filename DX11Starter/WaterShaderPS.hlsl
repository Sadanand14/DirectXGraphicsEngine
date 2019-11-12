cbuffer externalData : register(b0)
{
	float3 CameraPosition;
	
}

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


Texture2D waterTexture		: register (t0);
SamplerState Sampler		: register (s0);
Texture2D Scene				: register (t1);
SamplerState RefracSampler  : register (s1);

//ScreenSpace
Texture2D Reflection		:register (t2);


float4 main(WaterVertexToPixel input) : SV_TARGET
{

	input.Tangent = normalize(input.Tangent);
	input.Normal = normalize(input.Normal);

	//return float4(input.Normal, 1.0f);

	float indexOfRefraction = 0.95f;
	float adjustRefraction = 0.1f;

	float3 directionToPixel = normalize(input.worldPos - CameraPosition);
	float3 refractionDir = refract(directionToPixel, input.Normal, indexOfRefraction);

	float2 refracUV = mul(float4(refractionDir, 0.0f), input.View).xy * adjustRefraction;
	refracUV.x *= -1.0f;

	float4 SceneColor = Scene.Sample(RefracSampler, input.ScreenUV + refracUV);

	float4 finalColor = waterTexture.Sample(Sampler ,input.UV)*0.5f + SceneColor*0.5f ;
	float4 reflectionColor = Reflection.Sample(Sampler, input.UV);
	return finalColor;
}