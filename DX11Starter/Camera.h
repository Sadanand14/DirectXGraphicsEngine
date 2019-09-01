#pragma once
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "Mesh.h"
#include <d3dcompiler.h>
#include "Game.h"
#include"types.h"

using namespace DirectX;
class Camera 
{
	XMFLOAT4X4 veiwMatrix, projectionMatrix;
	XMVECTOR view,pos,upUnit;
	XMFLOAT3 currentPos,cameraDir;
	float xRot, yRot,delta;
public:
	Camera(float width, float height);
	~Camera();
	void Update(float deltaTime);
	XMFLOAT4X4 GetView() { return veiwMatrix; }
	XMFLOAT4X4 GetProjection() { return projectionMatrix; }
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUpward();
	void MoveDownward();
	void RotateLeft();
	void RotateRight();
	void RotateUp();
	void RotateDown();
};