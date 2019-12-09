#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"
#include "Camera.h"

struct GPUParticle 
{
	DirectX::XMFLOAT4 Color;
	float Age;
	DirectX::XMFLOAT3 position;
	float Size;
	DirectX::XMFLOAT3 velocity;
	float Alive;
	DirectX::XMFLOAT3 padding;
};

struct ParticleSort
{
	unsigned int index;
};

class GPUEmitter 
{
private:

	static float emitTimeCounter;
	unsigned int m_maxParticles, m_emitRate;
	float m_timePerEmit;
	SimpleComputeShader* m_initParticlesCS,* m_updateParticleCS,* m_emitParticleCS,* m_updateArgsBufferCS;
	SimpleVertexShader* m_VS;
	SimplePixelShader* m_PS;

	ID3D11Buffer* m_indexBuff = nullptr , * drawArgsBuff = nullptr;
	ID3D11UnorderedAccessView* particlePoolUAV = nullptr, * deadParticleUAV = nullptr , * drawParticleUAV = nullptr, * drawArgsUAV = nullptr;
	ID3D11ShaderResourceView* particlePoolSRV = nullptr, * deadParticleSRV = nullptr, *drawParticleSRV = nullptr;


public:

	GPUEmitter
	(
		unsigned int maxParticles, unsigned int emitRate, float lifeTime, ID3D11Device* device, ID3D11DeviceContext* context,
		SimpleComputeShader* initParticles, SimpleComputeShader* updateParticles, SimpleComputeShader* emitParticles, 
		SimpleComputeShader* updateArgsBuffer, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader
	);

	~GPUEmitter();

	void Update(float deltaTime, float totalTime);

	void Draw(float deltaTime , float totalTime);
};