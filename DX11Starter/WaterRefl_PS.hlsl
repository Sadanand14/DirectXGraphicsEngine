Texture2D SceneTex : register (t0);
Texture2D depthTex : register (t1);
SamplerState Sampler : register (s0);

struct VertexToPixel
{
	float4 Position		: SV_POSITION;
	float4 vsPos		: TEXCOORD0;
	float3 csPos		: TEXCOORD1;
	float3 vsNormal		: TEXCOORD2;
	matrix Proj			: PROJECTION;
};

float4 main(VertexToPixel In) : SV_TARGET
{
	////ScreenSpace Reflection Stuff/////////////////////////////////////////////////////

	float4 ProjectionParams;
	ProjectionParams.x = 1 / In.Proj[0][0];
	ProjectionParams.y = 1 / In.Proj[1][1];
	ProjectionParams.z = In.Proj[3][2];
	ProjectionParams.w = In.Proj[2][2];

	float EdgeDistThreshold = 0.2f;
	float ReflScale = 1.0f;
	float DepthBias = 1.6f;
	float PixelSize = 2.0 / 720.0f;
	float numChecks = 1280;
	float ViewAngleThreshold = 0.0f;

	// Pixel position and normal in view space
	float3 vsPos = In.Position.xyz;
	float3 vsNorm = normalize(In.vsNormal);
	// Calculate the camera to pixel direction
	float3 eyeToPixel = normalize(vsPos);
	// Calculate the reflected view direction
	float3 vsReflect = reflect(eyeToPixel, vsNorm);
	// The initial reflection color for the pixel
	float4 reflectColor = float4(0.0, 0.0, 0.0, 0.0);
	// Don't bother with reflected vector above the threshold vector
	if (vsReflect.z > ViewAngleThreshold)
	{
		// Fade the reflection as the view angles gets close to the threshold
		float viewAngleThresholdInv = 1.0 - ViewAngleThreshold;
		float viewAngleFade = (vsReflect.z - ViewAngleThreshold) / viewAngleThresholdInv;
		// Transform the View Space Reflection to clip-space
		float3 vsPosReflect = vsPos + vsReflect;
		float3 csPosReflect = mul(float4(vsPosReflect, 1.0), In.Proj).xyz / vsPosReflect.z;
		float3 csReflect = csPosReflect - In.csPos;
		// Resize Screen Space Reflection to an appropriate length.
		float reflectScale = PixelSize / length(csReflect.xy);
		csReflect *= reflectScale;
		// Calculate the offsets in texture space
		float3 currOffset = In.csPos + csReflect;
		currOffset.xy = currOffset.xy * float2(0.5, -0.5) + 0.5;
		float3 lastOffset = In.csPos;
		lastOffset.xy = lastOffset.xy * float2(0.5, -0.5) + 0.5;
		csReflect = float3(csReflect.x * 0.5, csReflect.y * -0.5,csReflect.z);
		// Iterate over the HDR texture searching for intersection
		for (int nCurStep = 0; nCurStep < numChecks; nCurStep++)
		{
			// Sample from depth buffer
			float curSample = depthTex.SampleLevel(Sampler, currOffset.xy, 0.0).z + DepthBias;
			curSample = ProjectionParams.z / (ProjectionParams.w + curSample);
			if (curSample < currOffset.z)
			{
				// Correct the offset based on the sampled depth
				currOffset.xy = lastOffset.xy + (currOffset.z - curSample)* csReflect.xy;
				// Get the HDR value at the location
				reflectColor.xyz = SceneTex.SampleLevel(Sampler,currOffset.xy, 0.0).xyz;
				// Fade out samples close to the texture edges
				float edgeFade = saturate(distance(currOffset.xy,float2(0.5, 0.5)) * 2.0 - EdgeDistThreshold);
				// Find the fade value
				reflectColor.w = min(viewAngleFade, 1.0 - edgeFade *edgeFade);
				// Apply the reflection sacle
				reflectColor.w *= ReflScale;
				// Advance past the final iteration to break the loop
				nCurStep = numChecks;
			}

			// Advance to the next sample
			lastOffset = currOffset;
			currOffset += csReflect;
		}
	}
	return reflectColor;
}