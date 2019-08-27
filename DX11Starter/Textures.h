#pragma once
#include "d3d11.h"
#include <string>

class Texture 
{
private:
	ID3D11ShaderResourceView* m_srv;

public:
	Texture(std::string, ID3D11Device* , ID3D11DeviceContext*);
	~Texture();

};