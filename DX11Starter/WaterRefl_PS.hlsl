
Texture2D SceneTex : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler : register (s0);

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	float3 vsNormal		: NORMAL;
	float3 csPos		: CLIPPOS;
	matrix Proj			: PROJECTION;
};

float4 GetReflection(float3 reflVec, float startDepth, float2 samplePos, matrix projection) 
{
	float4 color = (0.0f, 0.0f, 0.0f, 0.0f);
	float stepsize = 1 / 1920;
	float size = length(reflVec.xy);
	reflVec = normalize(reflVec/size);
	reflVec *= stepsize;
	float currDepth = startDepth;
	float sampleDepth = depthTex.Sample(Sampler, samplePos).x;
	sampleDepth = projection[3][2] / (sampleDepth + projection[2][2]);
	while (samplePos.x <= 1.0f && samplePos.x >= 0.0 && samplePos.y >= 0 && samplePos.y <= 1.0f) 
	{
		samplePos = samplePos + reflVec.xy;
		currDepth = currDepth + reflVec.z * startDepth;
		float sampledDepth = depthTex.SampleLevel(Sampler, samplePos, 0.0).x;
		sampledDepth = projection[3][2] / (sampledDepth + projection[2][2]);
		
		if (currDepth > sampledDepth) 
		{
			float delta = currDepth - sampledDepth;
			color = SceneTex.SampleLevel(Sampler, samplePos , 0.0);
		}
	}

	return color;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	////ScreenSpace Reflection Stuff/////////////////////////////////////////////////////

	float4 ProjectionParams;
	ProjectionParams.x = 1 / input.Proj[0][0];
	ProjectionParams.y = 1 / input.Proj[1][1];
	ProjectionParams.z = input.Proj[3][2];
	ProjectionParams.w = input.Proj[2][2];
	
	float4 depthValue = depthTex.Sample(Sampler, input.csPos);
	float linearDepth = ProjectionParams.z / (depthValue + ProjectionParams.w);

	//viewingDirection in viewSpace;
	float3 eyePosition = normalize(float3(0,0,1));
	float3 reflectDir = mul(float4( reflect(eyePosition, input.vsNormal), 1.0f), input.Proj).xyz;

	float4 reflectionColor = GetReflection(reflectDir, linearDepth , input.csPos.xy, input.Proj);

	return reflectionColor;
}