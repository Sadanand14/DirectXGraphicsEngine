struct WaterVertexToPixel 
{
	float4 Position : SV_POSITION;
	float Time : TIME;
};

float4 main(WaterVertexToPixel input) : SV_TARGET
{
	return float4(1.0f,0.0f+input.Time*0.05f ,0.0f,0.0f);
}