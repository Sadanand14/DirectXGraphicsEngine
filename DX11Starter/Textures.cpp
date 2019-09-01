#include "Textures.h"
#include "WICTextureLoader.h"
//#include <wstring>

ID3D11SamplerState*  Texture::m_sampler;

Texture::Texture(std::string path, ID3D11Device* device, ID3D11DeviceContext* context)
{
	DirectX::CreateWICTextureFromFile(device, context, (wchar_t*)path.c_str(), 0, &m_srv);

	if (!m_sampler) 
	{
		D3D11_SAMPLER_DESC samplerDesc = {}; // The {} part zeros out the struct!
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MaxAnisotropy = 16;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // Setting this allows for mip maps to work! (if they exist)
		// Ask DirectX for the actual object
		device->CreateSamplerState(&samplerDesc, &m_sampler);
	}
}

Texture::~Texture()
{
	m_srv->Release();
}