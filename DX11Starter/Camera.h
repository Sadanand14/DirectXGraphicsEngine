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
	XMVECTOR view,pos,upUnit;
	XMFLOAT3 currentPos,cameraDir;
	float xRot, yRot,delta;
public:
	Camera();
	~Camera();
	void Update(float deltaTime);
	XMFLOAT4X4 GetView() { return veiwMatrix; }
	XMFLOAT4X4 GetProjection() { return projectionMatrix; }
	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUpward(float deltaTime);
	void MoveDownward(float deltaTime);
	void RotateLeft();
	void RotateRight();
	void RotateUp(float deltaTime);
	void RotateDown(float deltaTime);
};