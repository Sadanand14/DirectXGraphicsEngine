cbuffer externalData : register (b0) 
{
	float3 CameraPosition;
}

Texture2D SceneTex : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler : register (s0);

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	float4 worldPos		: POSITION;
	float4 Normal 		: NORMAL;
	matrix Proj			: PROJECTION;
	float2 UV			: TEXCOORD3;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	////ScreenSpace Reflection Stuff/////////////////////////////////////////////////////

	//float4 ProjectionParams;
	//ProjectionParams.x = 1 / input.Proj[0][0];
	//ProjectionParams.y = 1 / input.Proj[1][1];
	//ProjectionParams.z = input.Proj[3][2];
	//ProjectionParams.w = input.Proj[2][2];
	//
	//float4 depthValue = depthTex.Sample(Sampler, input.UV);
	//float linearDepth = ProjectionParams.z / (depthValue + ProjectionParams.w);

	//float3 eyePosition = normalize(input.worldPos - CameraPosition);


	//float3 vsReflect = reflect(eyePosition, vsNormal);
	//float4 reflectColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//if (vsReflect.z > ViewAngleThreshold)
	//{
	//	//float invertedViewAngleThreshold = 1 - ViewAngleThreshold;	
	//	//float viewAngleFade = (vsReflect.z - ViewAngleThreshold) / invertedViewAngleThreshold;

	//	float proj = input.Proj;
	//	float3 vsPosReflect = vsPos + vsReflect;
	//	//vsPosReflect = reflect(vsPosReflect, float3(0.0f,1.0f,0.0f));
	//	float3 csPosReflect = mul(float4(vsPosReflect, 1.0f), proj).xyz / vsPosReflect.z;
	//	float3 csReflect = csPosReflect + input.csPos;

	//	float reflectionScale = PixelSize / length(csReflect.xy);
	//	//csReflect *= reflectionScale;// ReflScale;

	//	float3 currOffset = csReflect - input.csPos;
	//	currOffset.xy = currOffset.xy * float2(0.5f, -0.5f) + 0.5f;
	//	float3 lastOffset = input.csPos;
	//	lastOffset.xy = lastOffset.xy * float2(0.5f, -0.5f) + 0.5f;
	//	csReflect = float3(csReflect.x * 0.5f, csReflect.y * -0.5, csReflect.z);

	//	for (unsigned int i = 0; i < numChecks; i++)
	//	{
	//		float X = 1 - currOffset.x;
	//		float Y = 1 - currOffset.y;
	//		float depthSample = depthTex.SampleLevel(Sampler,float2(X,Y), 0.0).x + DepthBias;
	//		//depthSample = ProjectionParams.z / (depthSample + ProjectionParams.w);
	//		if (depthSample < currOffset.z)
	//		{
	//			//float2 uv = currOffset.xy;
	//			currOffset.xy = lastOffset.xy + (currOffset.z - depthSample) * csReflect.xy;
	//			
	//			reflectColor.xyz = SceneTex.SampleLevel(Sampler, float2(X,Y), 0.0).xyz;

	//			//float edgeFade = saturate(distance(currOffset.xy, float2(0.5f,0.5f))*2.0f )
	//			//
	//			reflectColor.w = 1.0f;
	//			i = numChecks;
	//		}
	//		lastOffset = currOffset;
	//		currOffset += csReflect;
	//	}
	//}
	return float4(0.0,0.0f,1.0f,1.0f);
}