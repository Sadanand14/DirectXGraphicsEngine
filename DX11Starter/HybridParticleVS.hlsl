cbuffer external : register(b0)
{
	matrix view;
	matrix projection;

	int startIndex;
	float3 acc;
	float4 startColor;
	float4 endColor;
	float startSize;
	float endSize;
	float lifeTime;
	float totalTime;
}

struct Particle
{
	float3 startPos;
	float spawnTime;

	float3 startVel;
	float rotStart;

	float rotEnd;
	float3 padding;
};


StructuredBuffer<Particle> ParticleBuff: register(t0);

struct VertexToPixel
{
	float4 Position	: SV_POSITION;
	float2 UV		: TEXCOORD;
	float4 Color	: COLOR;
};

VertexToPixel main(uint id: SV_VertexID) : SV_POSITION
{
	VertexToPixel output;

	uint particleID = id / 4;
	uint cornerID = id % 4;

	Particle particle = ParticleBuff.Load(particleID + startIndex);
	float t = totalTime - particle.spawnTime;
	float agePercent = t / lifeTime;

	float3 pos = acc * t * t / 2.0f + p.startVel * t + p.startPos;
	float4 color = lerp(startColor, endColor, agePercent);
	float size = lerp(startSize, endSize, agePercent);
	float rotation = lerp(p.rotStart, p.rotEnd, agePercent);

	float2 offsets[4];
	offsets[0] = float2(-1.0f, 1.0f);
	offsets[0] = float2(1.0f, 1.0f);
	offsets[0] = float2(1.0f, -1.0f);
	offsets[0] = float2(-1.0f, -1.0f);

	float Sine, Cosine;

	sincos(rotation, Sine, Cosine);
	float2x2 rotationMatrix =
	{
		Cosine, Sine, -Sine, Cosine
	};

	float2 rotatedOffset = mul(offsets[cornerID], rotationMatrix)

}