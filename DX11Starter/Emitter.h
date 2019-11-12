#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"


struct Particle
{
	DirectX::XMFLOAT3 m_startPosiiton,m_currPosition, m_startVeloctiy;
	DirectX::XMFLOAT4 m_Color;

	float m_size;
	float m_Age;
	float m_startRot;
	float m_endRot;
	float rotation;
};

struct ParticleVertex
{
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT4 m_Color;
	DirectX::XMFLOAT2 m_UV;
};

class Emitter
{
	DirectX::XMFLOAT3 m_startVelocity, m_startPosition, m_emitterPosition, m_emitterAcceleration;
	DirectX::XMFLOAT4 m_startColor, m_endColor;

	DirectX::XMFLOAT3 m_positionRange, m_velocityRange;
	DirectX::XMFLOAT4 m_rotationRange;

	unsigned int m_liveParticles,m_particlesPerSec;

	float m_timeSinceEmit, m_timePerEmmission, m_lifeTime, m_startSize, m_endSize;

	//particles array
	Particle* m_particleArr;
	unsigned int m_maxParticles, m_maxParticles, m_firstAlive;

	//VertexArray
	ParticleVertex* m_vertices;
	ID3D11Buffer* vbuff, * Ibuff;
	ID3D11ShaderResourceView* texture;
	SimpleVertexShader* vs;
	SimplePixelShader* ps;

public:

	Emitter();
	~Emitter();
	void UpdateEmitter();
	void DrawEmitter();
};