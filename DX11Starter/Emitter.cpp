#include "Emitter.h"

Emitter::Emitter(
	DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
	DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
	unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize 
	)
{
	m_maxParticles  = maxParticles;
	m_emitRate = emitRate;

}

Emitter::~Emitter() 
{
	
}

void Emitter::UpdateEmitter() 
{

}

void Emitter::DrawEmitter() 
{
	
}