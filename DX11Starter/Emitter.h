#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"
#include "Camera.h"


struct Particle
{
	DirectX::XMFLOAT3 m_startPosition,m_currPosition, m_startVeloctiy;
	DirectX::XMFLOAT4 m_Color;

	float m_size;
	float m_Age;
	float m_startRot;
	float m_endRot;
	float m_rotation;
};

struct ParticleVertex
{
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT2 m_UV;
};

class Emitter
{
	DirectX::XMFLOAT3 m_startVelocity, m_emitterPosition, m_emitterAcceleration;
	DirectX::XMFLOAT4 m_startColor, m_endColor;

	DirectX::XMFLOAT3 m_positionRange, m_velocityRange;
	DirectX::XMFLOAT4 m_rotationRange;

	unsigned int m_liveParticles,m_emitRate;

	float m_timeSinceEmit, m_timePerEmmission, m_lifeTime, m_startSize, m_endSize;

	//particles array
	Particle* m_particleArr;
	unsigned int m_maxParticles, m_oldestDead, m_oldestAlive;

	//VertexArray
	ParticleVertex* m_vertices;
	ID3D11Buffer* m_vbuff, * m_Ibuff;
	ID3D11ShaderResourceView* m_texture;
	SimpleVertexShader* m_vs;
	SimplePixelShader* m_ps;

	void SpawnParticle();
	void UpdateParticle(float delta, unsigned int particleIndex);
	void SetupGPU(ID3D11DeviceContext* context, Camera* camera);
	void SetupGPUParticle(unsigned int index, Camera* camera);
	XMFLOAT3 ParticleVertexPos(unsigned int index, unsigned int cornerIndex, Camera* camera);

public:

	Emitter
	(
		DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
		DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
		DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
		unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize,
		ID3D11Device* device, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* srv
	);
	~Emitter();

	void UpdateEmitter(float delta);
	void DrawEmitter(ID3D11DeviceContext* context, Camera* camera);
};
