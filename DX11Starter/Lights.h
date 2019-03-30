#pragma once
#include "d3d11.h"
#include <DirectXMath.h>
namespace DirectX {
	struct DirectionalLight
	{
		XMFLOAT4 AmbientColor;
		XMFLOAT4 DiffuseColor;
		XMFLOAT3 Direction;
	};

	struct PointLight 
	{
		XMFLOAT4 Position;
		XMFLOAT4 Colour;
	};
}