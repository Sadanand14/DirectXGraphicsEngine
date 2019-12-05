#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "SimpleShader.h"
#include "Camera.h"

class GPUEmitter 
{
private:

	unsigned int m_maxParticles, m_emitRate;
	float m_timePerEmit;
	SimpleComputeShader* m_initParticlesCS,* m_updateParticleCS,* m_emitParticleCS,* m_updateArgsBufferCS;
	SimpleVertexShader* m_VS;
	SimplePixelShader* m_PS;

public:

	GPUEmitter
	(
		unsigned int maxParticles, unsigned int emitRate, ID3D11Device* device, ID3D11DeviceContext* context,
		SimpleComputeShader* initParticles, SimpleComputeShader* updateParticles, SimpleComputeShader* emitParticles, 
		SimpleComputeShader* updateArgsBuffer, SimpleVertexShader* vertexShader, SimpleVertexShader* pixelShader
	);

	~GPUEmitter();

	void Update(float deltaTime, float totalTime);

	void Draw(float deltaTime , float totalTime);
};