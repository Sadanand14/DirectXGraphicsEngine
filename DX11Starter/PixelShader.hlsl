
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

SamplerState Sampler: register(s0);
Texture2D Texture : register(t0);

cbuffer externalData: register(b0)
{
	DirectionalLight Light;
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

float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 surfaceColor = Texture.Sample(Sampler,input.UV);
	float3 finalColor = CalculateLight(Light,input.Normal)*surfaceColor.rgb;
	return  float4(finalColor,1);
	
}

