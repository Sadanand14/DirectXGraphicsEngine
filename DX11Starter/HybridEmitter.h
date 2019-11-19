#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"
#include "Camera.h"

struct HybridParticle 
{
	DirectX::XMFLOAT3 StartPosition;
	float spawnTime;

	DirectX::XMFLOAT3 StartVelocity;
	float RotationStart;

	float RotationEnd;
	DirectX::XMFLOAT3 padding;
};

class HybridEmitter 
{
private:
	DirectX::XMFLOAT3 m_startVel, m_emitterAcc, m_emitterPos;
	DirectX::XMFLOAT3 m_velRange, m_posRange;
	
	DirectX::XMFLOAT4 m_rotRange, m_startColor, m_endColor;

	unsigned int m_emitRate, m_liveParticles, m_maxParticles, m_oldestAlive, m_oldestDead;
	float m_timePerEmission, m_timeSinceEmit, m_lifeTime, m_startSize, m_endSize;

	ID3D11Buffer* m_particleBuff, *m_Ibuff;
	ID3D11ShaderResourceView* m_particleBuffSRV, * m_texture;

	SimpleVertexShader* m_vs;
	SimplePixelShader* m_PS;

	void UpdateParticle(unsigned int index, float totalTime);
	void SpawnParticle(float totalTime);

public:
	HybridEmitter
	(
		DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
		DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
		DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
		unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize,
		ID3D11Device* device, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* srv
	);

	~HybridEmitter();

	void UpdateEmitter(float delta, float TotalTime);
	void DrawEmitter(ID3D11DeviceContext* context, Camera* camera, float totalTime);
};