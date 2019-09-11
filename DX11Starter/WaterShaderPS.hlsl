struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
};

float4 main(WaterVertexToPixel input) : SV_TARGET
{
	return float4(0.6f,0.5f,0.0f,0.0f);
}