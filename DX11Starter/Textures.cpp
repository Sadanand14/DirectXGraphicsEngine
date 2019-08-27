#include "Textures.h"
#include "WICTextureLoader.h"
#include <sstream>
//#include <wstring>

Texture::Texture(std::string path, ID3D11Device* device, ID3D11DeviceContext* context)
{
	DirectX::CreateWICTextureFromFile(device, context, (wchar_t*)path.c_str(), 0, &m_srv);
}

Texture::~Texture()
{

}