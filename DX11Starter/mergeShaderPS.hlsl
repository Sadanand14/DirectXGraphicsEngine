struct vertexToPixel
{
	float4 position : SV_POSITION;
	float2 UV		: TEXCOORD;
};

Texture2D rawImage :register(t0);
Texture2D blurrImage :register(t1);

float4 main(vertexToPixel input) : SV_TARGET
{
	float4 mainColor = rawImage.Load(int3(input.position.x , input.position.y, 0));
	float4 blurrColor = blurrImage.Load(int3(input.position.x, input.position.y, 0));
	float4 output = float4(0.0f,0.0f,0.0f,0.0f);
	if (length(blurrColor)==0.0f) 
	{
		output += mainColor;
	}
	else 
	{
		output += blurrColor;
	}
	
	return blurrColor;
}