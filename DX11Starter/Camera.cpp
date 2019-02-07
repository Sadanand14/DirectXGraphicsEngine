#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Camera.h"

using namespace DirectX;

Camera::Camera() 
{
	view = XMVectorSet(0,0,1,0);
	pos = XMVectorSet(0,0,-5,0);
	upUnit = XMVectorSet(0, 1, 0, 0);
	XMMATRIX vm = XMMatrixLookToLH(pos, view, upUnit);
	XMStoreFloat4x4(&veiwMatrix,XMMatrixTranspose(vm));
	xRot = 0.0f;
	yRot = 0.0f;
}

Camera::~Camera() 
{
	
}

void Camera::Update(float deltaTime) 
{
	delta = deltaTime;
	view = XMVector3Rotate(view, XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f));
	XMMATRIX vm = XMMatrixLookToLH(pos,view, upUnit);
	XMStoreFloat4x4(&veiwMatrix, XMMatrixTranspose(vm));
}

void Camera::MoveForward(float deltaTime)
{
	pos = XMVectorAdd(pos,XMVector3Normalize(view));
	
}

void Camera::MoveBackward(float deltaTime)
{
	XMVECTOR reverseDir = XMVectorScale(view, (-1));
	pos = XMVectorAdd(pos, XMVector3Normalize(reverseDir));

}

void Camera::MoveLeft(float deltaTime)
{
	XMVECTOR cross = XMVector3Cross(upUnit, view);
	pos = XMVectorAdd(pos, XMVector3Normalize(cross));
	
}

void Camera::MoveRight(float deltaTime)
{
	XMVECTOR cross = XMVector3Cross(view, upUnit);
	pos = XMVectorAdd(pos, XMVector3Normalize(cross));
	
}


void Camera::MoveUpward(float deltaTime)
{
	XMVECTOR dir = XMVectorScale(upUnit, deltaTime);
	pos = XMVectorAdd(pos, XMVector3Normalize(dir));

}

void Camera::MoveDownward(float deltaTime)
{
	XMVECTOR reverseDir = XMVectorScale(upUnit, (-1*deltaTime));
	pos = XMVectorAdd(pos, XMVector3Normalize(reverseDir));

}

void Camera::RotateLeft()
{
	xRot += 0.1f;
	
}

void Camera::RotateRight()
{
	xRot -= 0.1f;
	
}

void Camera::RotateUp(float deltaTime)
{
	yRot += 0.1f*deltaTime;

}
void Camera::RotateDown(float deltaTime)
{
	yRot -= 0.1f*deltaTime;

}