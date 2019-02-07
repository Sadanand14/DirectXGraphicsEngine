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

Camera::~Camera() {	}

void Camera::Update(float deltaTime) 
{
	delta = deltaTime;
	view = XMVector3Rotate(view, XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f));
	XMMATRIX vm = XMMatrixLookToLH(pos,view, upUnit);
	XMStoreFloat4x4(&veiwMatrix, XMMatrixTranspose(vm));
}

void Camera::MoveForward()
{
	XMVECTOR moveDir = XMVectorScale(view, delta*0.0001);
	pos = XMVectorAdd(pos,XMVector3Normalize(moveDir));
}

void Camera::MoveBackward()
{
	XMVECTOR reverseDir = XMVectorScale(view, (-1 * delta*0.0001));
	pos = XMVectorAdd(pos, XMVector3Normalize(reverseDir));
}

void Camera::MoveLeft()
{
	XMVECTOR cross = XMVector3Cross(upUnit, view);
	cross = XMVectorScale(cross,delta*0.0001);
	pos = XMVectorAdd(pos, XMVector3Normalize(cross));
}

void Camera::MoveRight()
{
	XMVECTOR cross = XMVector3Cross(view, upUnit);
	cross = XMVectorScale(cross, delta*0.0001);
	pos = XMVectorAdd(XMVector3Normalize(cross), pos);
}


void Camera::MoveUpward()
{
	XMVECTOR dir = XMVectorScale(upUnit, delta*0.0001);
	pos = XMVectorAdd(pos, XMVector3Normalize(dir));
}

void Camera::MoveDownward()
{
	XMVECTOR reverseDir = XMVectorScale(upUnit, (-1* delta*0.0001));
	pos = XMVectorAdd(pos, XMVector3Normalize(reverseDir));
}

void Camera::RotateLeft()
{
	xRot += 0.1f*0.01;
}

void Camera::RotateRight()
{
	xRot -= 0.1f*0.01;
}

void Camera::RotateUp()
{
	yRot += 0.1f*delta*0.0001;
}
void Camera::RotateDown()
{
	yRot -= 0.1f*delta*0.0001;
}