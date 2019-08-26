
TextureCube tex: register(t0);
SamplerState Sampler: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 direction	: NORMAL;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return tex.Sample(Sampler, input.direction);
}
