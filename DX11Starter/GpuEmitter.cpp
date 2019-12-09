#include "GpuEmitter.h"

float GPUEmitter::emitTimeCounter = 0;

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
	poolDesc.ByteWidth = sizeof(GPUParticle) * m_maxParticles;
	poolDesc.CPUAccessFlags = 0;
	poolDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	poolDesc.StructureByteStride = sizeof(GPUParticle);
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
	ID3D11Buffer* deadBuffer;
	D3D11_BUFFER_DESC deadDesc = {};
	deadDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	deadDesc.ByteWidth = sizeof(unsigned int) * m_maxParticles;
	deadDesc.CPUAccessFlags = 0;
	deadDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	deadDesc.StructureByteStride = sizeof(unsigned int);
	deadDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateBuffer(&deadDesc, 0, &deadBuffer);

	D3D11_UNORDERED_ACCESS_VIEW_DESC deadUAVDesc = {};
	deadUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	deadUAVDesc.Buffer.FirstElement = 0;
	deadUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	deadUAVDesc.Buffer.NumElements = m_maxParticles;
	deadUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

	device->CreateUnorderedAccessView(deadBuffer, &deadUAVDesc, &deadParticleUAV);

	deadBuffer->Release();

	
	
	
	//DrawList
	ID3D11Buffer* drawBuff;
	D3D11_BUFFER_DESC drawDesc = {};
	drawDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS|D3D11_BIND_SHADER_RESOURCE;
	drawDesc.ByteWidth = sizeof(ParticleSort) * m_maxParticles;
	drawDesc.CPUAccessFlags = 0;
	drawDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	drawDesc.StructureByteStride = sizeof(ParticleSort);
	drawDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateBuffer(&drawDesc, 0, &drawBuff);

	D3D11_UNORDERED_ACCESS_VIEW_DESC drawUAVDesc = {};
	drawUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	drawUAVDesc.Buffer.FirstElement = 0;
	drawUAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
	drawUAVDesc.Buffer.NumElements = m_maxParticles;
	drawUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	device->CreateUnorderedAccessView(drawBuff, &drawUAVDesc, &drawParticleUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC drawSRVDesc = {};
	drawSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	drawSRVDesc.Buffer.FirstElement = 0;
	drawSRVDesc.Buffer.NumElements = m_maxParticles;
	drawSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	device->CreateShaderResourceView(drawBuff, &drawSRVDesc, &drawParticleSRV);

	drawBuff->Release();




	//draw Argument Buffer
	D3D11_BUFFER_DESC drawArgsDesc = {};
	drawArgsDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	drawArgsDesc.ByteWidth = sizeof(unsigned int) * 5;
	drawArgsDesc.CPUAccessFlags = 0;
	drawArgsDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	drawArgsDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateBuffer(&drawDesc, 0, &drawBuff);

	D3D11_UNORDERED_ACCESS_VIEW_DESC drawArgUAVDesc = {};
	drawArgUAVDesc.Format = DXGI_FORMAT_R32_UINT;
	drawArgUAVDesc.Buffer.FirstElement = 0;
	drawArgUAVDesc.Buffer.Flags = 0;
	drawArgUAVDesc.Buffer.NumElements = 5;
	drawArgUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	device->CreateUnorderedAccessView(drawArgsBuff, &drawArgUAVDesc, &drawArgsUAV);

}

GPUEmitter::~GPUEmitter() 
{
	m_indexBuff->Release();
	particlePoolSRV->Release();
	particlePoolUAV->Release();
}