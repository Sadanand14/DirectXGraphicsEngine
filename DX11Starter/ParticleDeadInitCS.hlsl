cbuffer ExternalData : register(b0)
{
	int maxParticles;
}

AppendStructuredBuffer<uint> DeadList : register(u0);
[numthreads(32, 1, 1)]
void main( uint3 id: SV_DispatchThreadID )
{
	if (id.x >= (uint)maxParticles) return;

	DeadList.Append(id.x);
}