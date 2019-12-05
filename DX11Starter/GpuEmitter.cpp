#include "GpuEmitter.h"

GPUEmitter::GPUEmitter
(
	unsigned int maxParticles, unsigned int emitRate, ID3D11Device* device, ID3D11DeviceContext* context,
	SimpleComputeShader* initParticles, SimpleComputeShader* updateParticles, SimpleComputeShader* emitParticles,
	SimpleComputeShader* updateArgsBuffer, SimpleVertexShader* vertexShader, SimpleVertexShader* pixelShader
) 
{
	
}