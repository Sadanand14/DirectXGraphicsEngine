Texture2D SceneTex : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler : register (s0);

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	//float4 vsPos		: TEXCOORD0;
	//float3 csPos		: TEXCOORD1;
	//float3 vsNormal		: TEXCOORD2;
	//matrix Proj			: PROJECTION;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	////ScreenSpace Reflection Stuff/////////////////////////////////////////////////////

	//float4 ProjectionParams;
	//ProjectionParams.x = 1 / input.Proj[0][0];
	//ProjectionParams.y = 1 / input.Proj[1][1];
	//ProjectionParams.z = input.Proj[3][2];
	//ProjectionParams.w = input.Proj[2][2];

	//float ReflScale = 1.0f;
	//float DepthBias = 0.0f;
	//float PixelSize = 2.0 / 720.0f;
	//float numChecks = 1280;
	//float ViewAngleThreshold = 0.0f;

	//float3 vsPos = input.vsPos.xyz;
	////float3 cameraPos = mul(float4 (CameraPosition, 1.0f), input.WVP);
	//float3 vsNormal = normalize(input.vsNormal);
	//float3 dirToPixel = normalize(vsPos);

	//float3 vsReflect = reflect(dirToPixel, vsNormal);

	//float4 reflectColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//if (vsReflect.z > ViewAngleThreshold)
	//{
	//	//float invertedViewAngleThreshold = 1 - ViewAngleThreshold;	
	//	//float viewAngleFade = (vsReflect.z - ViewAngleThreshold) / invertedViewAngleThreshold;

	//	float proj = input.Proj;
	//	float3 vsPosReflect = vsPos + vsReflect;
	//	float3 ssPosReflect = mul(float4(vsPosReflect, 1.0f), proj).xyz / vsPosReflect.z;
	//	float3 ssReflect = ssPosReflect - input.csPos;

	//	float reflectionScale = PixelSize / length(ssReflect.xy);
	//	ssReflect *= ReflScale;

	//	float3 currOffset = input.csPos + ssReflect;
	//	currOffset.xy = currOffset.xy * float2(0.5f, -0.5f) + 0.5f;
	//	float3 lastOffset = input.csPos;
	//	lastOffset.xy = lastOffset.xy * float2(0.5f, -0.5f) + 0.5f;
	//	ssReflect = float3(ssReflect.x * 0.5f, ssReflect.y * -0.5, ssReflect.z);

	//	for (unsigned int i = 0; i < numChecks; i++)
	//	{
	//		float depthSample = depthTex.SampleLevel(Sampler, currOffset.xy, 0.0).x + DepthBias;
	//		depthSample = ProjectionParams.z / (depthSample + ProjectionParams.w);
	//		if (depthSample < currOffset.z)
	//		{
	//			//float2 uv = currOffset.xy;
	//			currOffset.xy = lastOffset.xy + (currOffset.z - depthSample) * ssReflect.xy;
	//			reflectColor.xyz = SceneTex.SampleLevel(Sampler, currOffset.xy, 0.0).xyz;

	//			//float edgeFade = saturate(distance(currOffset.xy, float2(0.5f,0.5f))*2.0f )
	//			//
	//			reflectColor.w = 1.0f;
	//			i = numChecks;
	//		}
	//		lastOffset = currOffset;
	//		currOffset += ssReflect;
	//	}
	//}
	return float4(1.0f,1.0f,1.0f,1.0f);
}