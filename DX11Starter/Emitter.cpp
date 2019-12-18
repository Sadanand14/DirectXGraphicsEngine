#include "Emitter.h"
#include <iostream>
#include <vector>
Emitter::Emitter(
	DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
	DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
	unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize,
	ID3D11Device* device, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* srv
)
{
	m_maxParticles = maxParticles;
	m_emitRate = emitRate;
	m_startVelocity = startVelocity;
	m_startColor = startColor;
	m_endColor = endColor;
	m_rotationRange = rotationRange;
	m_velocityRange = velocityRange;
	m_positionRange = positionRange;
	m_emitterPosition = emitterPosition;
	m_emitterAcceleration = emitterAcceleration;
	m_lifeTime = lifeTime;
	m_startSize = startSize;
	m_endSize = endSize;
	m_texture = srv;
	m_vs = vs;
	m_ps = ps;

	m_timePerEmmission = 1.0f / emitRate;

	m_particleArr = new Particle[m_maxParticles];
	ZeroMemory(m_particleArr, sizeof(Particle) * m_maxParticles);

	m_oldestAlive = 0;
	m_oldestDead = 0;
	m_liveParticles = 0;
	m_timeSinceEmit = 0;

	//m_vertices = new ParticleVertex[4];
	//m_vertices[0].m_UV = DirectX::XMFLOAT2(0.0f, 0.0f);
	//m_vertices[1].m_UV = DirectX::XMFLOAT2(1.0f, 0.0f);
	//m_vertices[2].m_UV = DirectX::XMFLOAT2(1.0f, 1.0f);
	//m_vertices[3].m_UV = DirectX::XMFLOAT2(0.0f, 1.0f);
	m_vertices = new ParticleVertex[4 * m_maxParticles];
	for (unsigned int i = 0; i < m_maxParticles * 4; i += 4)
	{
		m_vertices[i].m_UV = DirectX::XMFLOAT2(0.0f, 0.0f);
		m_vertices[i + 1].m_UV = DirectX::XMFLOAT2(1.0f, 0.0f);
		m_vertices[i + 2].m_UV = DirectX::XMFLOAT2(1.0f, 1.0f);
		m_vertices[i + 3].m_UV = DirectX::XMFLOAT2(0.0f, 1.0f);
	}

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth = sizeof(ParticleVertex) * 4 * m_maxParticles;
	//vertexDesc.ByteWidth = sizeof(ParticleVertex) * 4;
	device->CreateBuffer(&vertexDesc, 0, &m_vbuff);


	//std::vector<unsigned int> indexArr = {0,1,2,0,2,3};
	//unsigned int* indexArr = new unsigned int[6];
	unsigned int* indexArr = new unsigned int[m_maxParticles * 6];
	unsigned int index = 0;
	for (unsigned int i = 0; i < 4 * m_maxParticles; i += 4)
	//for (unsigned int i = 0; i < 4 ; i += 4)
	{
		indexArr[index++] = i;
		indexArr[index++] = i + 1;
		indexArr[index++] = i + 2;
		indexArr[index++] = i;
		indexArr[index++] = i + 2;
		indexArr[index++] = i + 3;
	}
	/*indexArr[0] = 0;
	indexArr[1] = 1;
	indexArr[2] = 2;
	indexArr[3] = 0;
	indexArr[4] = 2;
	indexArr[5] = 3;*/


	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indexArr;

	CD3D11_BUFFER_DESC indexDesc = {};
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexDesc.ByteWidth = sizeof(unsigned int) * 6;
	indexDesc.ByteWidth = sizeof(unsigned int) * 6 * m_maxParticles;

	device->CreateBuffer(&indexDesc, &indexData, &m_Ibuff);
	delete[] indexArr;

	for(unsigned int i = 0 ; i < m_emitRate;i++)
		SpawnParticle();
}

Emitter::~Emitter()
{
	delete[] m_particleArr;
	delete[] m_vertices;
	m_vbuff->Release();
	m_Ibuff->Release();
}

void Emitter::UpdateParticle(float delta, unsigned int particleIndex)
{
	Particle* currentParticle = m_particleArr + particleIndex;
	if (currentParticle->m_Age >= m_lifeTime)
		return;

	currentParticle->m_Age += delta;
	if (currentParticle->m_Age >= m_lifeTime)
	{
		m_oldestAlive++;
		m_oldestAlive %= m_maxParticles;
		m_liveParticles--;
		return;
	}

	using namespace DirectX;
	float agePercentage = currentParticle->m_Age / m_lifeTime;
	XMStoreFloat4(&currentParticle->m_Color,
		XMVectorLerp(XMLoadFloat4(&m_startColor), XMLoadFloat4(&m_endColor), agePercentage));

	currentParticle->m_rotation = currentParticle->m_startRot +
		agePercentage * (currentParticle->m_endRot - currentParticle->m_startRot);

	currentParticle->m_size = m_startSize+ agePercentage * (m_endSize - m_startSize);
	currentParticle->m_currPosition = m_emitterPosition;

	XMVECTOR startPos = XMLoadFloat3(&currentParticle->m_startPosition);
	XMVECTOR startVelocity = XMLoadFloat3(&currentParticle->m_startVeloctiy);
	XMVECTOR acc = XMLoadFloat3(&m_emitterAcceleration);
	float t = currentParticle->m_Age;

	XMVECTOR pos = ((acc * t * t) / 2.0f) + (startVelocity * t) + startPos;

	XMStoreFloat3(&currentParticle->m_currPosition, pos);
}

void Emitter::UpdateEmitter(float delta)
{
	//delta /= 100;
	if (m_oldestAlive < m_oldestDead)
	{
		for (unsigned int i = m_oldestAlive; i < m_oldestDead; i++)
		{
			UpdateParticle(delta, i);
		}
	}
	else
	{
		for (unsigned int i = m_oldestAlive; i < m_maxParticles; i++)
		{
			UpdateParticle(delta, i);
		}
		for (unsigned int i = 0; i < m_oldestDead; i++)
		{
			UpdateParticle(delta, i);
		}
	}

	m_timeSinceEmit += delta;

	while (m_timeSinceEmit > m_timePerEmmission)
	{
		SpawnParticle();
		m_timeSinceEmit -= m_timePerEmmission;
	}
}

void Emitter::SpawnParticle()
{
	if (m_liveParticles == m_maxParticles)
		return;

	using namespace DirectX;
	Particle* firstDeadOne = m_particleArr + m_oldestDead;
	//Particle* firstDeadOne = m_particleArr;
	firstDeadOne->m_Age = 0;
	firstDeadOne->m_size = m_startSize;
	firstDeadOne->m_Color = m_startColor;

	firstDeadOne->m_startPosition = m_emitterPosition;
	firstDeadOne->m_startPosition.x += (((float)rand() / RAND_MAX) * 2 - 1) * m_positionRange.x;
	firstDeadOne->m_startPosition.y += (((float)rand() / RAND_MAX) * 2 - 1) * m_positionRange.y;
	firstDeadOne->m_startPosition.z += (((float)rand() / RAND_MAX) * 2 - 1) * m_positionRange.z;

	firstDeadOne->m_currPosition = firstDeadOne->m_startPosition;

	firstDeadOne->m_startVeloctiy = m_startVelocity;
	firstDeadOne->m_startVeloctiy.x += (((float)rand() / RAND_MAX) * 2 - 1) * m_velocityRange.x;
	firstDeadOne->m_startVeloctiy.y += (((float)rand() / RAND_MAX) * 2 - 1) * m_velocityRange.y;
	firstDeadOne->m_startVeloctiy.z += (((float)rand() / RAND_MAX) * 2 - 1) * m_velocityRange.z;

	firstDeadOne->m_startRot = ((float)rand() / RAND_MAX) *
		(m_rotationRange.y - m_rotationRange.x) + m_rotationRange.x;

	firstDeadOne->m_endRot = ((float)rand() / RAND_MAX) *
		(m_rotationRange.w - m_rotationRange.z) + m_rotationRange.z;

	m_liveParticles++;

	++m_oldestDead;
	m_oldestDead %= m_maxParticles;

}

void Emitter::DrawEmitter(ID3D11DeviceContext* context, Camera* camera)
{
	SetupGPU(context, camera);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vbuff, &stride, &offset);
	context->IASetIndexBuffer(m_Ibuff, DXGI_FORMAT_R32_UINT, 0);

	m_vs->SetMatrix4x4("view", camera->GetView());
	m_vs->SetMatrix4x4("projection", camera->GetProjection());
	m_vs->SetShader();
	m_vs->CopyAllBufferData();

	m_ps->SetShaderResourceView("particleTex", m_texture);
	//m_ps->CopyAllBufferData();
	m_ps->SetShader();

	//std::cout << "Live: " << m_liveParticles << " firstAlive : " << m_firstAlive << " firstDead: " << m_firstDead << "\n";

	if (m_oldestAlive < m_oldestDead)
	{
		context->DrawIndexed(m_liveParticles * 6, m_oldestAlive * 6, 0);
	}
	else
	{
		context->DrawIndexed((m_oldestAlive-1) * 6, 0, 0);

		context->DrawIndexed((m_maxParticles - m_oldestAlive) * 6, m_oldestAlive * 6, 0);
	}

	context->DrawIndexed(6,0,0);

}

void Emitter::SetupGPU(ID3D11DeviceContext* context, Camera* camera)
{
	if (m_oldestAlive < m_oldestDead)
	{
		for (unsigned int i = m_oldestAlive; i < m_oldestDead; i++)
		{
			SetupGPUParticle(i, camera);
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_oldestDead; i++)
		{
			SetupGPUParticle(i, camera);
		}
		for (unsigned int i = m_oldestAlive; i < m_maxParticles; i++)
		{
			SetupGPUParticle(i, camera);
		}
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(m_vbuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	memcpy(mapped.pData, m_vertices, sizeof(ParticleVertex) * 4 * m_maxParticles);
	//memcpy(mapped.pData, m_vertices, sizeof(ParticleVertex) * 4 );
	context->Unmap(m_vbuff, 0);
}

void Emitter::SetupGPUParticle(unsigned int index, Camera* camera)
{
	unsigned int i = index * 4;
	m_vertices[i].m_Position = ParticleVertexPos(index, 0, camera);
	m_vertices[i + 1].m_Position = ParticleVertexPos(index, 1, camera);
	m_vertices[i + 2].m_Position = ParticleVertexPos(index, 2, camera);
	m_vertices[i + 3].m_Position = ParticleVertexPos(index, 3, camera);

	m_vertices[i].m_Color = m_particleArr[index].m_Color;
	m_vertices[i + 1].m_Color = m_particleArr[index].m_Color;
	m_vertices[i + 2].m_Color = m_particleArr[index].m_Color;
	m_vertices[i + 3].m_Color = m_particleArr[index].m_Color;
}

XMFLOAT3 Emitter::ParticleVertexPos(unsigned int index, unsigned int cornerIndex, Camera* camera)
{
	XMFLOAT4X4 view = camera->GetView();
	XMVECTOR cameraRight = XMVectorSet(view._11, view._12, view._13, 0);
	XMVECTOR cameraUp = XMVectorSet(view._21, view._22, view._23, 0);

	XMFLOAT2 Offset = m_vertices[index * 4 + cornerIndex].m_UV;
	Offset.x = Offset.x * 2 - 1;
	Offset.y = (Offset.y * (-2) + 1);

	XMVECTOR offsetVec = XMLoadFloat2(&Offset);
	XMMATRIX rotationMatrix = XMMatrixRotationZ(m_particleArr[index].m_rotation);
	offsetVec = XMVector3Transform(offsetVec, rotationMatrix);

	XMVECTOR posVec = XMLoadFloat3(&m_particleArr[index].m_currPosition);
	posVec += cameraRight * XMVectorGetX(offsetVec) * m_particleArr[index].m_size;
	posVec += cameraUp * XMVectorGetY(offsetVec) * m_particleArr[index].m_size;

	XMFLOAT3 position;
	XMStoreFloat3(&position, posVec);
	//XMFLOAT3(m_vertices[index].m_Position);
	return position;
}