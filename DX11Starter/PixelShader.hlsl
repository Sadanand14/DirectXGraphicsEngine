
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage


struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	//float4 color		: COLOR;
	float3 Normal		: NORMAL;
	float2 UV           : TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight 
{
	float3 Position;
	float3 Color;
};

SamplerState Sampler: register(s0);
Texture2D Texture : register(t0);

cbuffer externalData: register(b0)
{
	DirectionalLight Light;
	PointLight pointLight;
	float3 cameraPosition;
}

//helper function for calculating directional light from each light on the surface
float4 CalculateLight(DirectionalLight dL, float3 normal)
{
	normal = normalize(normal);
	float3 nlight = normalize(dL.Direction*(-1));
	float dotproduct = saturate(dot(normal, nlight));
	float4 finalColor = dotproduct * dL.DiffuseColor + dL.AmbientColor;
	return finalColor;
}

//float4 CalculatePointLight(PointLight pl, float3 normal)
//{
//	input.normal = normalize(input.normal);
//
//	// Requirements for lighting...
//	float3 surfaceColor = float3(1,	0, 1);
//	float shininess = 32.0f; // Arbitrary surface shininess value
//
//	float3 dirToCamera = normalize(CameraPosition - input.worldPos);
//	float3 dirToPointLight = normalize(pointLight.Position - input.worldPos);
//
//	float3 pointNdotL = dot(intput.normal, dirToPointLight);
//	pointNdotL = saturate(pointNdotL);
//	
//	float3 pointReflec = reflect(-dirToPointLight, input.normal);
//	float pointSpec = pow(saturate(dot(pointReflec, dirToCamera)), shininess);
//	float 3 finalPointLight = surfaceColor * PointLightColor*pointNdotL + pointSpec.rrr;
//}
float4 main(VertexToPixel input) : SV_TARGET
{
	input.Normal = normalize(input.Normal);

	// Requirements for lighting...
	float3 surfaceColorPL = float3(1, 0, 1);
	float shininess = 32.0f; // Arbitrary surface shininess value

	float3 dirToCamera = normalize(cameraPosition - input.position);
	float3 dirToPointLight = normalize(pointLight.Position - input.position);

	float3 pointNdotL = dot(input.Normal, dirToPointLight);
	pointNdotL = saturate(pointNdotL);

	float3 pointReflec = reflect(-dirToPointLight, input.Normal);
	float pointSpec = pow(saturate(dot(pointReflec, dirToCamera)), shininess);
	float3 finalPointLight = surfaceColorPL * pointLight.Color*pointNdotL + pointSpec.rrr;
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 surfaceColor = Texture.Sample(Sampler,input.UV);
	float3 finalColor = CalculateLight(Light,input.Normal)*surfaceColor.rgb;
	return  float4(finalColor+finalPointLight, 1);
}

