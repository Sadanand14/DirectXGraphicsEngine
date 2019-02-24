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
	xRot = 0.0f;
	yRot = 0.0f;
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
}

Camera::~Camera() {	}

void Camera::Update(float deltaTime)
{
	delta = deltaTime;

	view = XMVector3Rotate(XMVectorSet(0,0,1,0), XMLoadFloat4(&rotation) );
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

void Camera::MouseLook(float x, float y) 
{
	xRot += y*0.002f;
	yRot += x*0.002f;
	xRot = max(min(xRot, XM_PIDIV2), -XM_PIDIV2);
	XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f));
}