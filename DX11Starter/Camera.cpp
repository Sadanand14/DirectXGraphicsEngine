#include "Game.h"
#include "Vertex.h"
#include "d3d11.h"
#include "Mesh.h"
#include <vector>
#include "Camera.h"

using namespace DirectX;

Camera::Camera(float width,float height) 
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		width / height,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
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
	if (xRot > 0)
	{
		xRot -= 0.0001f*delta;
	}
	if (xRot < 0)
	{
		xRot += 0.0001f*delta;
	}
	if (yRot > 0) 
	{
		yRot -= 0.0001f*delta;
	}
	if (yRot < 0)
	{
		yRot += 0.0001f*delta;
	}
	view = XMVector3Rotate(view, XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f));
	XMMATRIX vm = XMMatrixLookToLH(pos,view, upUnit);
	XMStoreFloat4x4(&veiwMatrix, XMMatrixTranspose(vm));
}

void Camera::MoveForward()
{
	XMVECTOR moveDir = XMVectorScale(view, delta);
	pos = XMVectorAdd(pos,moveDir);
}

void Camera::MoveBackward()
{
	XMVECTOR reverseDir = XMVectorScale(view, (-1 * delta));
	pos = XMVectorAdd(pos, reverseDir);
}

void Camera::MoveLeft()
{
	XMVECTOR cross = XMVector3Cross(view, upUnit);
	cross = XMVectorScale(cross, delta);
	pos = XMVectorAdd(pos, cross);
}

void Camera::MoveRight()
{
	XMVECTOR cross = XMVector3Cross(upUnit, view);
	cross = XMVectorScale(cross, delta);
	pos = XMVectorAdd(cross, pos);
}


void Camera::MoveUpward()
{
	XMVECTOR dir = XMVectorScale(upUnit, delta);

	pos = XMVectorAdd(pos, dir);
}

void Camera::MoveDownward()
{
	XMVECTOR reverseDir = XMVectorScale(upUnit, (-1* delta));
	pos = XMVectorAdd(pos, reverseDir);
}

void Camera::RotateLeft()
{
	yRot -= 0.5f*delta;
}

void Camera::RotateRight()
{
	yRot += 0.5f*delta;
}

void Camera::RotateUp()
{
	xRot += 0.001f*delta;
}
void Camera::RotateDown()
{
	xRot -= 0.001f*delta;
}