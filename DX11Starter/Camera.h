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
	XMFLOAT4 rotation;
	float xRot, yRot,delta;
public:
	Camera(float width, float height);
	~Camera();
	void Update(float deltaTime);
	inline XMFLOAT4 GetPos() {
		XMFLOAT4 POS;
		XMStoreFloat4(&POS, pos);
		return POS; 
	}
	inline XMFLOAT4X4 GetView() { return veiwMatrix; }
	inline XMFLOAT4X4 GetProjection() { return projectionMatrix; }
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUpward();
	void MoveDownward();
	void MouseLook(float, float);
};