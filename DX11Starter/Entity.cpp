#include "Mesh.h"
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "Game.h"
#include "Materials.h"

Entity::Entity(XMMATRIX a, XMMATRIX b, XMMATRIX c, Mesh* d, Materials* e, DirectionalLight f)
{
	XMStoreFloat4x4(&translation, a);
	XMStoreFloat4x4(&rotation, b);
	XMStoreFloat4x4(&scaling, c);
	mesh = d;
	material = e;
	light = f;
}

Entity::~Entity() 
{
	mesh = nullptr; 
}

XMMATRIX Entity::GetWM()
{
	XMMATRIX trans, rot, scale, result;
	trans = XMLoadFloat4x4(&translation);
	rot = XMLoadFloat4x4(&rotation);
	scale = XMLoadFloat4x4(&scaling);
	result = XMMatrixMultiply(XMMatrixMultiply(scale, rot), trans);
	return result;
}