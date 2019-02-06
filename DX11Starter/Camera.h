#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"

using namespace DirectX;
class Camera 
{
	XMFLOAT4X4 veiwMatrix, projectionMatrix;
	XMFLOAT3 currentPos, cameraDir;
	float xRot, yRot;
public:
	Camera();
	~Camera();
	void Update();
	XMFLOAT4X4 GetView() { return veiwMatrix; }
	XMFLOAT4X4 GetProjection() { return projectionMatrix; }
};