#include"HybridEmitter.h"

HybridEmitter::HybridEmitter
(
	DirectX::XMFLOAT3 startVelocity, DirectX::XMFLOAT4 startColor, DirectX::XMFLOAT4 endColor,
	DirectX::XMFLOAT4 rotationRange, DirectX::XMFLOAT3 velocityRange, DirectX::XMFLOAT3 positionRange,
	DirectX::XMFLOAT3 emitterPosition, DirectX::XMFLOAT3 emitterAcceleration,
	unsigned int maxParticles, unsigned int emitRate, float lifeTime, float startSize, float endSize,
	ID3D11Device* device, SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* srv
) 
{

}

HybridEmitter::~HybridEmitter() 
{
	
}

void HybridEmitter::UpdateEmitter(float delta, float totalTime) 
{
	
}

void HybridEmitter::UpdateParticle(unsigned int index, float totalTIme) 
{
	
}

void HybridEmitter::DrawEmitter(ID3D11DeviceContext* context, Camera* camera, float totalTime)
{

}


void HybridEmitter::SpawnParticle(float totalTime) 
{
	
}
