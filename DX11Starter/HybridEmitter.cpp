#include"HybridEmitter.h"
#include <iostream>
HybridEmitter::HybridEmitter
(
	DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
	DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
	unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize,
	ID3D11Device* device, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* srv
)
{
	m_startVel = startVelocity;
	m_startColor = startColor;
	m_endColor = endColor;
	m_rotRange = rotationRange;
	m_velRange = velocityRange;
	m_posRange = positionRange;
	m_emitterPos = emitterPosition;
	m_emitterAcc = emitterAcceleration;

	m_maxParticles = maxParticles;
	m_emitRate = emitRate;
	m_lifeTime = lifeTime;
	m_startSize = startSize;
	m_endSize = endSize;

	m_timePerEmission = 1.0f / m_emitRate;
	m_aliveHead = 0; 
	m_deadHead = 0; 
	m_liveParticles = 0;
	m_timeSinceEmit = 0;

	m_vs = vs;
	m_ps = ps;

	m_texture = srv;

	m_particleArr = new HybridParticle[m_maxParticles];
	ZeroMemory(m_particleArr, sizeof(HybridParticle) * m_maxParticles);

	unsigned int* m_indexArr = new unsigned int[6 * m_maxParticles];
	unsigned int index = 0;
	for (unsigned int i = 0; i < 4 * m_maxParticles; i += 4)
	{
		m_indexArr[index++] = i;
		m_indexArr[index++] = i + 1;
		m_indexArr[index++] = i + 2;
		m_indexArr[index++] = i;
		m_indexArr[index++] = i + 2;
		m_indexArr[index++] = i + 3;
	}

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = m_indexArr;

	D3D11_BUFFER_DESC indexDesc = {};
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(unsigned int) * 6 * m_maxParticles;
	device->CreateBuffer(&indexDesc, &indexData, &m_Ibuff);

	D3D11_BUFFER_DESC particleBuffDesc = {};
	particleBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	particleBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	particleBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	particleBuffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	particleBuffDesc.StructureByteStride = sizeof(HybridParticle);
	particleBuffDesc.ByteWidth = sizeof(HybridParticle) * m_maxParticles;
	device->CreateBuffer(&particleBuffDesc, 0, &m_particleBuff);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = maxParticles;
	device->CreateShaderResourceView(m_particleBuff, &srvDesc, &m_particleBuffSRV);
	
	delete[]m_indexArr;
}

HybridEmitter::~HybridEmitter()
{
	delete[] m_particleArr;
	m_Ibuff->Release();
	m_particleBuff->Release();
	m_particleBuffSRV->Release();
}

void HybridEmitter::UpdateEmitter(float delta, float totalTime)
{
	if (m_liveParticles > 0) 
	{
		if (m_aliveHead < m_deadHead)
		{
			for (unsigned int i = m_aliveHead; i < m_deadHead; i++)
				UpdateParticle(i, totalTime);
		}
		else if (m_deadHead<m_aliveHead)
		{
		
			for (unsigned int i = 0; i < m_deadHead; i++) 
				UpdateParticle(i, totalTime);

			for (unsigned int i = m_aliveHead; i < m_maxParticles; i++) 
				UpdateParticle(i, totalTime);
		}
		else 
		{
			for (unsigned int i = 0; i < m_maxParticles; i++) 
				UpdateParticle(i, totalTime);
		}
	}
	
	m_timeSinceEmit += delta;

	while (m_timeSinceEmit > m_timePerEmission) 
	{
		SpawnParticle(totalTime);
		m_timeSinceEmit -= m_timePerEmission;
	}
}

void HybridEmitter::UpdateParticle(unsigned int index, float totalTime)
{
	float age = totalTime - m_particleArr[index].spawnTime;
	if (age >= m_lifeTime) 
	{
		++m_aliveHead %= m_maxParticles;
		m_liveParticles--;
	}
}

void HybridEmitter::DrawEmitter(ID3D11DeviceContext* context, Camera* camera, float totalTime)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(m_particleBuff, 0, D3D11_MAP_WRITE_DISCARD, 0 , &mapped);
	memcpy(mapped.pData, m_particleArr, sizeof(HybridParticle) * m_maxParticles);
	context->Unmap(m_particleBuff, 0);

	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* nullbuffer = 0;
	context->IASetVertexBuffers(0, 1, &nullbuffer, &stride, &offset);
	context->IASetIndexBuffer(m_Ibuff, DXGI_FORMAT_R32_UINT, 0);

	m_vs->SetMatrix4x4("view", camera->GetView());
	m_vs->SetMatrix4x4("projection", camera->GetProjection());

	m_vs->SetFloat3("acc", m_emitterAcc);
	m_vs->SetFloat4("startColor", m_startColor);
	m_vs->SetFloat4("endColor", m_endColor);
	m_vs->SetFloat("startSize", m_startSize);
	m_vs->SetFloat("endSize", m_endSize);
	m_vs->SetFloat("lifeTime", m_lifeTime);
	m_vs->SetFloat("totalTime", totalTime);

	m_vs->SetShader();

	context->VSSetShaderResources(0, 1, &m_particleBuffSRV);
	m_ps->SetShaderResourceView("particleTex", m_texture);
	m_ps->SetShader();

	std::cout << "live : " << m_liveParticles << " m_oldestAlive : " << m_aliveHead << " oldestDead : " << m_deadHead << "\n";

	if (m_aliveHead < m_deadHead)
	{
		m_vs->SetInt("startIndex", m_aliveHead);
		m_vs->CopyAllBufferData();
		context->DrawIndexed(m_liveParticles * 6, 0, 0);
	}
	else 
	{
		m_vs->SetInt("startIndex", 0);
		m_vs->CopyAllBufferData();
		context->DrawIndexed(m_deadHead*6,0,0);

		m_vs->SetInt("startIndex", m_aliveHead);
		m_vs->CopyAllBufferData();
		context->DrawIndexed((m_maxParticles-m_aliveHead)*6,0,0);
	}
}


void HybridEmitter::SpawnParticle(float totalTime)
{
	if (m_liveParticles == m_maxParticles)
		return;

	HybridParticle* currParticle = m_particleArr + m_deadHead;

	currParticle->spawnTime = totalTime;

	currParticle->StartPosition = m_emitterPos;
	currParticle->StartPosition.x += (((float)rand() / RAND_MAX) * 2 - 1) * m_posRange.x;
	currParticle->StartPosition.y += (((float)rand() / RAND_MAX) * 2 - 1) * m_posRange.y;
	currParticle->StartPosition.z += (((float)rand() / RAND_MAX) * 2 - 1) * m_posRange.z;

	currParticle->StartVelocity = m_startVel;
	currParticle->StartVelocity.x += (((float)rand() / RAND_MAX) * 2 - 1) * m_velRange.x;
	currParticle->StartVelocity.y += (((float)rand() / RAND_MAX) * 2 - 1) * m_velRange.y;
	currParticle->StartVelocity.z += (((float)rand() / RAND_MAX) * 2 - 1) * m_velRange.z;

	currParticle->RotationStart = ((float)rand() / RAND_MAX) * (m_rotRange.y - m_rotRange.x) + m_rotRange.x;

	currParticle->RotationEnd = ((float)rand() / RAND_MAX) * (m_rotRange.w - m_rotRange.z) + m_rotRange.z;

	++m_deadHead %= m_maxParticles;
	m_liveParticles++;

}
