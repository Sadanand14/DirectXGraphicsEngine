#pragma once
#include "d3d11.h"
#include <DirectXMath.h>
namespace DirectX {
	struct DirectionalLight
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT4 direction;
	};
}