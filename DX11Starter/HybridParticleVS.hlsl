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

VertexToPixel main(uint id: SV_VertexID)
{
	VertexToPixel output;

	uint particleID = id / 4;
	uint cornerID = id % 4;

	Particle particle = ParticleBuff.Load(particleID + startIndex);
	float t = totalTime - particle.spawnTime;
	float agePercent = t / lifeTime;

	float3 pos = acc * t * t / 2.0f + particle.startVel * t + particle .startPos;
	float4 color = lerp(startColor, endColor, agePercent);
	float size = lerp(startSize, endSize, agePercent);
	float rotation = lerp(particle.rotStart, particle.rotEnd, agePercent);

	float2 offsets[4];
	offsets[0] = float2(-1.0f, 1.0f);
	offsets[1] = float2(1.0f, 1.0f);
	offsets[2] = float2(1.0f, -1.0f);
	offsets[3] = float2(-1.0f, -1.0f);

	float Sine, Cosine;

	sincos(rotation, Sine, Cosine);
	float2x2 rotationMatrix =
	{
		Cosine, Sine, -Sine, Cosine
	};

	float2 rotatedOffset = mul(offsets[cornerID], rotationMatrix);
	pos += float3 (view._11, view._21, view._31) * rotatedOffset.x * size;
	pos += float3 (view._12, view._22, view._32) * rotatedOffset.y * size;

	matrix viewProjection = mul(view, projection);
	output.Position = mul(float4(pos, 1.0f), viewProjection);

	float2 UV[4];
	UV[0] = float2(0, 0);
	UV[1] = float2(1, 0);
	UV[2] = float2(1, 1);
	UV[3] = float2(0, 1);


	output.UV = saturate(UV[cornerID]);
	output.Color = color;
	return output;
}