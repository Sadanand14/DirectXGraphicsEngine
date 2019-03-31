
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
	float3 worldPos     : POSITION;
	float2 UV			: TEXCOORD;
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
	float4 Position;
	float4 AmbientColor;
	float4 SourceColor;
};

SamplerState Sampler: register(s0);
Texture2D Texture : register(t0);

cbuffer externalData: register(b0)
{
	DirectionalLight directionalLight;
	PointLight pointLight;
	float4 cameraPosition; 
}

//DIRECTIONAL LIGHT 
float3 GetDirLight(DirectionalLight dL, float4 surfaceColor,float3 cameraDir, float3 normal, float shine)
{
	//Directional Light part
	normal = normalize(normal);
	float3 normalizedDir = normalize(dL.Direction);
	float dirNdotL = dot(normal, -normalizedDir);
	dirNdotL = saturate(dirNdotL);
	float3 finalColor = surfaceColor * dL.DiffuseColor*dirNdotL + dL.AmbientColor;
	return finalColor;
}

//SPECULAR REFLECTION
float3 GetSpec(float3 lightDirection, float3 cameraDir, float3 normal,float shine)
{
	lightDirection = normalize(lightDirection);
	float3 pointReflec = reflect(-lightDirection, normal);
	float3 pointSpec = pow(saturate(dot(pointReflec, cameraDir)), shine);
	return pointSpec.rrr;
}

//POINTLIGHT
float3 GetPointLight(PointLight pl, float4 color, float3 pixelPos, float3 cameraDir, float3 normal, float shine)
{
	float3 dirToPointLight = normalize(pl.Position - pixelPos);
	float3 pointNdotL = dot(normal, dirToPointLight);
	pointNdotL = saturate(pointNdotL);
	float3 spec1 = GetSpec(dirToPointLight, cameraDir, normal, shine);
	float3 finalPointLight = color * pl.SourceColor*pointNdotL+spec1+ pl.AmbientColor;
	return finalPointLight;
}



float4 main(VertexToPixel input) : SV_TARGET
{
	// Requirements for lighting...
	input.Normal = normalize(input.Normal);
	float4 surfaceColor = Texture.Sample(Sampler, input.UV);
	float shine = 64.0f;
	float3 dirToCamera = normalize(cameraPosition.xyz - input.worldPos);

	//float3 finalDirLight = GetDirLight(directionalLight, surfaceColor, dirToCamera, input.Normal, shine);
	float3 finalPointLight = GetPointLight(pointLight, surfaceColor, input.worldPos, dirToCamera, input.Normal, shine);
	
	
	return  float4(finalPointLight, 1);
}

