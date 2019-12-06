#include "GpuEmitter.h"

GPUEmitter::GPUEmitter
(
	unsigned int maxParticles, unsigned int emitRate, float lifeTime, ID3D11Device* device, ID3D11DeviceContext* context,
	SimpleComputeShader* initParticles, SimpleComputeShader* updateParticles, SimpleComputeShader* emitParticles,
	SimpleComputeShader* updateArgsBuffer, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader
) 
{
	m_maxParticles = maxParticles;
	m_emitRate = emitRate;
	m_initParticlesCS = initParticles;
	m_updateParticleCS = updateParticles;
	m_emitParticleCS = emitParticles;
	m_updateArgsBufferCS = updateArgsBuffer;
	m_VS = vertexShader;
	m_PS = pixelShader;

	m_timePerEmit = 1.0f / emitRate;

	emitTimeCounter = 0.0f;

	//index buffer creation
	D3D11_BUFFER_DESC ibDesc = {};
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.ByteWidth = sizeof(unsigned long) * 6 * m_maxParticles;
	ibDesc.CPUAccessFlags = 0;
	ibDesc.MiscFlags = 0;
	ibDesc.StructureByteStride = 0;
	ibDesc.Usage = D3D11_USAGE_DEFAULT;

	unsigned long * indexArr = new unsigned long[m_maxParticles * 6];
	unsigned long index = 0;
	for (unsigned long i = 0; i < m_maxParticles; i++) 
	{
		indexArr[index++] = 0 + 4 * i;
		indexArr[index++] = 1 + 4 * i;
		indexArr[index++] = 2 + 4 * i;
		indexArr[index++] = 0 + 4 * i;
		indexArr[index++] = 2 + 4 * i;
		indexArr[index++] = 3 + 4 * i;
	}

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexArr;
	device->CreateBuffer(&ibDesc, &indexData, &m_indexBuff);

	delete[] indexArr;

	//create particle pool SRV and UAV
	ID3D11Buffer* particlePoolBuff;
	D3D11_BUFFER_DESC poolDesc = {};
	poolDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	poolDesc.ByteWidth = sizeof(Particle) * m_maxParticles;
	poolDesc.CPUAccessFlags = 0;
	poolDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	poolDesc.StructureByteStride = sizeof(Particle);
	poolDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateBuffer(&poolDesc, 0, &particlePoolBuff);

	D3D11_UNORDERED_ACCESS_VIEW_DESC poolUAVDesc = {};
	poolUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	poolUAVDesc.Buffer.FirstElement = 0;
	poolUAVDesc.Buffer.Flags = 0;
	poolUAVDesc.Buffer.NumElements = m_maxParticles;
	poolUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	device->CreateUnorderedAccessView(particlePoolBuff, &poolUAVDesc, & particlePoolUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC poolSRVDesc = {};
	poolSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	poolSRVDesc.Buffer.FirstElement = 0;
	poolSRVDesc.Buffer.NumElements = m_maxParticles;
	poolSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	device->CreateShaderResourceView(particlePoolBuff, &poolSRVDesc, &particlePoolSRV);

	particlePoolBuff->Release();

	//DeadList
	ID3D11Buffer* drawListBuffer;
}

GPUEmitter::~GPUEmitter() 
{
	m_indexBuff->Release();
	particlePoolSRV->Release();
	particlePoolUAV->Release();
}